#include "RevolutionMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "LineMapping.h"
#include <float.h>

#ifdef GETLENGTH
#define GET_LENGTH dimension
#else
#define GET_LENGTH getLength
#endif

RevolutionMapping::
RevolutionMapping() : Mapping(3,3,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  Default Constructor
//===========================================================================
{ 
  uninitialized=TRUE;   // *** not used any more ***
  RevolutionMapping::className="RevolutionMapping";
  setName( Mapping::mappingName,"revolutionMapping");
  setBasicInverseOption(canInvert);  // basicInverse is available. Not available if we rotate a 3D mapping.

  revAxis1=0;
  revAxis2=1;
  revAxis3=2;

  setGridDimensions( axis1,0 );  // these will be set in initialize
  setGridDimensions( axis2,0 );
  setGridDimensions( axis3,0 );

  revolutionary=NULL;
  startAngle=0.;
  endAngle=1.;
  lineOrigin.redim(3);
  lineOrigin=0.; lineOrigin(0)=-1.;   // revolve about a line that passes through this point, (-1,0,0)
  lineTangent.redim(3);
  lineTangent=0.; lineTangent(1)=1.; // revolve about the y-axis by default
  signForTangent=+1.;

  setIsPeriodic(axis3,functionPeriodic );  
  setBoundaryCondition( Start,revAxis3,-1 );
  setBoundaryCondition(   End,revAxis3,-1 );
}

RevolutionMapping::
RevolutionMapping(Mapping & revolutionary_, 
		    const real startAngle_ /* =0. */, 
		    const real endAngle_ /* =1. */ ,
		    const RealArray & lineOrigin_ /* =Overture::nullRealDistributedArray() */ ,
		    const RealArray & lineTangent_ /* =Overture::nullRealDistributedArray() */
		    )
   : Mapping(3,3,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  This constructor takes a mapping to revolve plus option parameters
/// \param revolutionary_ (input) : mapping to revolve.
/// \param startAngle_ (input) : starting "angle" (in [0,1]) for the reolution.
/// \param endAngle_ (input) : ending "angle" (in [0,1]) for the revolution.
/// \param lineOrigin_ (input) : the point of origin for the line of revolution.
/// \param lineTangent_ (input) : the tangent to the line of revolution.
//===========================================================================
{
  uninitialized=TRUE;
  RevolutionMapping::className="RevolutionMapping";
  // *wdh* do later revolutionary=&revolutionary_;

  revolutionary=NULL;  // reference count ??

  setName(mappingName,aString("revolution-")+revolutionary_.getName(mappingName));

  if( revolutionary_.getDomainDimension()==2 )
  {
    revAxis1=0;
    revAxis2=1;
    revAxis3=2;
  }
  else
  {
    // setDomainDimension(2);
    revAxis1=0;
    revAxis2=2; // Not used when revolving a curve
    revAxis3=1;
  }
  
  for( int axis=0; axis<revolutionary_.getDomainDimension()+1; axis++ )
      setGridDimensions( axis,0 );  // these will be set in initialize

  setRevolutionAngle(startAngle_,endAngle_);

  setLineOfRevolution(lineOrigin_,lineTangent_);

  setRevolutionary(revolutionary_);
}

void RevolutionMapping::
initialize()
// ========================================================================================
// /Description:
// Initialization routine.
//    This routine should be called when
//        1. The revolutionary is changed
//        2. the line of revolution is changed
//        3. The startAngle or endAngle are changed.
// ========================================================================================
{
  if( revolutionary==NULL )
    return;

  uninitialized=FALSE;
  
  //
  // determine the "handed-ness" of lineTangent X ( x -lineOrigin) 
  // This is used in the map and basicInverse functions.
  // We try to rotate the same way, even if the lineTangent -> -lineTangent.
  RealArray r(1,3), x(1,3);  r=.5;

//
// AP: the following needs to be redone for general lineOrigin & lineTangent
//
  if( revolutionary->getRangeDimension()==2 && lineTangent(2)==0. && lineOrigin(2)==0. )
  {
    #ifdef USE_PPP
      revolutionary->mapS(r,x);  // evaluate the un-revolved mapping at a point.
    #else
      revolutionary->map(r,x);  // evaluate the un-revolved mapping at a point.
    #endif

    signForTangent=(lineTangent(0)*(x(0,1)-lineOrigin(1)) - lineTangent(1)*(x(0,0)-lineOrigin(0)));
    if( fabs(signForTangent) < REAL_EPSILON*100. )
    {
      // try again
      r=.55;
      #ifdef USE_PPP
        revolutionary->mapS(r,x);
      #else
        revolutionary->map(r,x);
      #endif
      signForTangent=(lineTangent(0)*(x(0,1)-lineOrigin(1)) - lineTangent(1)*(x(0,0)-lineOrigin(0)));
      if( fabs(signForTangent) < REAL_EPSILON*100. )
      {
        printf("RevolutionMapping::initialize:ERROR in computing the signForTangent. Something is wrong here\n");
        printf("RevolutionMapping::initializ:The inverse Mapping may be wrong...\n");
      }
    }
    signForTangent = signForTangent > 0. ? -1. : +1.;
  }
  else
  {
    signForTangent = 1;
  }
  
  // Check to see if the end of the body of revolution has a cylindrical singularity on either end
  // the singular edge can lie on either axis1 or axis2 of the revolutionary
  bool singularityFound=false;
  if( domainDimension==3 )
  {
    for( int axis=axis1; axis<=axis2 && !singularityFound; axis++ ) 
    {
      for( int side=Start; side<=End; side++ )
      {
	r=0.;
	r(0,axis)=(real)side;
        #ifdef USE_PPP
          revolutionary->mapS(r,x);
        #else
           revolutionary->map(r,x);
        #endif

	// check that the two end points lie on the line of revolution
	real distToRevLine = fabs( (x(0,0)-lineOrigin(0))*lineTangent(1) - (x(0,1)-lineOrigin(1))*lineTangent(0) );
	   
        real xDist = sqrt( SQR(x(0,0)-lineOrigin(0)) + SQR(x(0,1)-lineOrigin(1)) );
        const real xEps = max(REAL_EPSILON*1000.*xDist, REAL_MIN*1000.);

        printF("RevolutionMapping: check for polar singularity on (side,axis)=(%i,%i) pt=(%9.3e,%9.3e) distToRevLine=%8.2e, "
               "xDist=%8.2e eps=%8.2e, xEps=%8.2e\n",side,axis,x(0,0),x(0,1),distToRevLine,xDist,REAL_EPSILON*100.,xEps);

	if( distToRevLine< xDist*1.e-4 && distToRevLine>xEps )
	{
	 printF("RevolutionMapping:WARNING: The end point (%9.3e,%9.3e) (side=%i,axis=%i) of the revolution mapping lies close to the \n"
                "  line of revolution, dist=%8.2e, but is greater than the expected tolerance of xEps=%8.2e,\n"
                "  and thus I will assume that there is NO polar singularity. \n",x(0,0),x(0,1),side,axis,distToRevLine,xEps);
	}
        // printF(" Rev: distToRevLine=%8.2e, xDist=%8.2e eps=%8.2e, xEps=%8.2e\n",distToRevLine,xDist,REAL_EPSILON*100.,xEps);
	
	
	// if( distToRevLine< REAL_EPSILON*100. ) *wdh* 090203 -- need to scale this epsilon 
	if( distToRevLine < xEps )
	{
	    printF("RevolutionMapping:INFO: The end point (%9.3e,%9.3e) (side=%i,axis=%i) lies on the axis of"
                   " revolution\n",x(0,0),x(0,1),side,axis);

          // When we revolve a 2D "grid" we must check the other end of the face to check that it also
          // lies on the axis
          const int axisp = 1-axis; 
	  r(0,axisp)=1.;
          #ifdef USE_PPP
            revolutionary->mapS(r,x);
          #else
             revolutionary->map(r,x);
          #endif

          distToRevLine = fabs( (x(0,0)-lineOrigin(0))*lineTangent(1) - (x(0,1)-lineOrigin(1))*lineTangent(0) );
	  xDist = sqrt( SQR(x(0,0)-lineOrigin(0)) + SQR(x(0,1)-lineOrigin(1)) );
	  
	  // if( distToRevLine < REAL_EPSILON*100. ) *wdh* 090203 -- need to scale this epsilon 
	  if( distToRevLine>= xEps )
	  {
	    printF("RevolutionMapping:WARNING: The other end of the face, point=(%9.3e,%9.3e) (side,axis)=(%i,%i) "
                   " does not lie on the axis to within the the expected tolerance\n",x(0,0),x(0,1),side,axisp);
	  }
	  else
	  {
	    // cylindrical singularity at r_0=0
	    singularityFound=true;
	    if( getTypeOfCoordinateSingularity( side,revAxis1 ) != polarSingularity )
	    {
	      printF("RevolutionMapping::info: Polar singularity found along on side=%i of axis=%i\n",side,axis);
	      if( revAxis1==0 && revAxis2==1 && revAxis3==2 )
	      {
		printF("RevolutionMapping::info: Coordinate axes are being re-ordered to be like the that of a sphere\n");
		printF("RevolutionMapping::info: axis1=axial (phi), axis2=theta, axis3=radial\n");
		setParameterAxes(axis,2,(axis+1)%2);
	      }
	      setTypeOfCoordinateSingularity( side,revAxis1,polarSingularity ); // phi has a "polar" singularity
	      setCoordinateEvaluationType( spherical,true );  // Mapping can be evaluated in spherical coordinates
	      if( getBoundaryCondition(side,revAxis1)<0 )
		setBoundaryCondition(side,revAxis1,0);
	    }
	  }
	}
      }
    }
    if( !singularityFound && ( 
      getTypeOfCoordinateSingularity( Start,revAxis1 ) == polarSingularity 
      || getTypeOfCoordinateSingularity( End  ,revAxis1 ) == polarSingularity ) )
    {
      setTypeOfCoordinateSingularity( Start,revAxis1,noCoordinateSingularity ); 
      setTypeOfCoordinateSingularity( End  ,revAxis1,noCoordinateSingularity ); 
      setCoordinateEvaluationType( spherical,FALSE);  // Mapping can be evaluated in spherical coordinates

      // no singularity found but mapping still thinks it has one, unset this
      printF("RevolutionMapping::info: Coordinate axes are being re-ordered to the default since the singularity has been removed\n");
      setParameterAxes(0,1,2);
    }
  }
  else if ( domainDimension==2 )
  { // kkc check for singularities on surfaces of revolution
    RealArray r(1,1);
    for( int side=Start; side<=End; side++ )
    {
      r=(real)side;
      #ifdef USE_PPP
        revolutionary->mapS(r,x);
      #else
        revolutionary->map(r,x);
      #endif
      if ( revolutionary->getRangeDimension()==2 )
	x(0,2) = 0.;

      if ( fabs( (x(0,1)-lineOrigin(1))*lineTangent(2) - (x(0,2)-lineOrigin(2))*lineTangent(1))< REAL_EPSILON*100.  &&
	   fabs( (x(0,0)-lineOrigin(0))*lineTangent(2) - (x(0,2)-lineOrigin(2))*lineTangent(0))< REAL_EPSILON*100.  &&
	   fabs( (x(0,0)-lineOrigin(0))*lineTangent(1) - (x(0,1)-lineOrigin(1))*lineTangent(0))< REAL_EPSILON*100. )
      {
	cout<<"RevolutionMapping::info : Polar singularity found for a surface of revolution on side="<<side<<endl;
	setTypeOfCoordinateSingularity(side,revAxis1,polarSingularity);
      }
    }
  }

  mappingHasChanged();
  // AP: setbounds does not seem to compute the bounds correctly?  *wdh* seems to work
  setBounds();

}

void RevolutionMapping::
setBounds()
// ========================================================================================
// /Description:
// Set the bounds on the mapping
//    This routine should be called when the mapping changes, currently by initialize
// ========================================================================================
{

  if( true )
  {
    getGrid();  // *wdh* use this, the code below fails for cab.igs surface 11 
    return;
  }
/* -----  
  // ** this still fails sometimes *** wdh   cab.igs surface 11 

  Range D(0,revolutionary->getDomainDimension()-1);
  Range R(0,revolutionary->getRangeDimension()-1);

  int n,axis;
  realArray r_ends(2,D.getLength()),x_ends(2,3);
  int nsamples;
  realArray r,x;

  if ( revolutionary->getDomainDimension() == 1 ) 
    {
      nsamples = 21;
      r.redim(nsamples,1);
      x.redim(nsamples,3);

      // gather some samples along the curve to estimate to maximum distance from
      // the line of revolution
      real dr = 1./real(nsamples-1);
      for ( n=0; n<nsamples; n++ )
	r(n,0) = n*dr;
      revolutionary->map(r,x);
    }
  else if ( revolutionary->getDomainDimension() == 2 )
    {
      x = revolutionary->getGrid();
      nsamples = x.getLength(0)*x.getLength(1)*x.getLength(2);
      x.reshape(nsamples,x.getLength(3));
    }

  realArray unitTangent = lineTangent/sqrt(sum(lineTangent*lineTangent));
  
  // find the maximum radius from the line of revolution, this will help us
  // define the bounding cylinder and from that the bounding box.
  real maxRad2 = 0;
  int maxIndex;
  realArray normVect;
  realArray poles(2,R.getLength());
  for ( n=0; n<nsamples; n++ )
    {
      real d = 0;
      for ( axis=0; axis<=R.getBound(); axis++ ) 
	d += (x(n,axis)-lineOrigin(axis))*unitTangent(axis);
      realArray linePt;
      linePt = lineOrigin(R) + d*unitTangent(R);
      linePt.reshape(1,R.getLength());
      real rad2 = sum(pow(x(n,R)-linePt(0,R),2));
      if ( rad2>maxRad2 )
	{
	  maxRad2 = rad2;
	  maxIndex = n;
	  normVect = x(n,R)-linePt(0,R); 
	}
      if ( n==0 )
	poles(0,R) = linePt(0,R);
      else if ( n==(nsamples-1) )
	poles(1,R) = linePt(0,R);
    }
  normVect = -normVect/sqrt(sum(pow(normVect,2))); // normalize and reverse the radial direction
  normVect.reshape(R);
  real rad  = sqrt(maxRad2);
  for ( axis=0; axis<R.getLength(); axis++ )
    {
      setRangeBound(Start, axis, min(poles(0,axis)+rad*normVect(axis), poles(1,axis)-rad*normVect(axis),
				     poles(1,axis)+rad*normVect(axis), poles(1,axis)-rad*normVect(axis)));
      setRangeBound(End,   axis, max(poles(0,axis)+rad*normVect(axis), poles(0,axis)-rad*normVect(axis),
				     poles(1,axis)+rad*normVect(axis), poles(1,axis)-rad*normVect(axis)));
    }
------- */

}

// Copy constructor is deep by default
RevolutionMapping::
RevolutionMapping( const RevolutionMapping & map, const CopyType copyType )
{
  RevolutionMapping::className="RevolutionMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "RevolutionMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

RevolutionMapping::
~RevolutionMapping()
{ 
  if( debug & 4 )
    cout << " RevolutionMapping::Desctructor called" << endl;

  if( revolutionary->decrementReferenceCount()==0 )
    delete revolutionary;
  
}

RevolutionMapping & RevolutionMapping::
operator=( const RevolutionMapping & X )
{
  if( RevolutionMapping::className != X.getClassName() )
  {
    cout << "RevolutionMapping::operator= ERROR trying to set a RevolutionMapping = to a" 
	 << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  revolutionary =X.revolutionary;
  if( revolutionary!=NULL )
    revolutionary->incrementReferenceCount(); // *wdh* 030508
  
  startAngle    =X.startAngle;
  endAngle      =X.endAngle;
  lineOrigin    =X.lineOrigin;
  lineTangent   =X.lineTangent;
  revAxis1      =X.revAxis1;
  revAxis2      =X.revAxis2;
  revAxis3      =X.revAxis3;
  signForTangent=X.signForTangent;
  uninitialized =X.uninitialized;
  return *this;
}

//---------------------------------------------------------------------------------------
//      3D rotation about the line through the point
//           lineOrigin(0:2):=x0
//      with tangent 
//            lineTangent(0:2):=(v1,v2,v3)
//
//  Method: 
//     First compute the mapping to revolve:
//                 r(0:1,I) -> (y(0:1,I),0)
//   Now decompose (y-x0) into a component parallel to the line and a component
//   orthogonal to the line:
//               y-x0 = a + b          a is the component parallel to (v1,v2,v3)
//   Then rotate the part orthogonal to the line:
//               x-x0 = a + R*b    R = rotation matrix
//
//  To do this compute c, a vector orthogonal to b and v:
//               c = v X b
//  Then
//       R*b = cos(theta)*b + sin(theta)*c
//  where
//             theta = r(2,I)*delta+startAngle*twoPi, delta=(endAngle-startAngle)*twoPi
//  Thus
//         x = a + cos(theta)*b + sin(theta)*c + x0
//             a = (y-x0,v) v
//             b = y-x0-a
//  and
//     d(x)/dr_i = d(a)/dr + cos()*d(b)/dr_i + sin()*d(c)/dr_i    i=0,1
//         d(a)/dr_i = (d(y)/dr_i,v) v
//         d(b)/dr_i = d(y)/dr_i - d(a)/dr_i
//         d(c)/dr_i = v X db/dr_i
//
//     d(x)/dr_2 = delta*( -sin()*b +cos()*c )
//---------------------------------------------------------------------------------------

void RevolutionMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
{
  if( revolutionary==NULL )
  {
    cout << "RevolutionMapping::map: Error: The mapping to be revolved has not been defined yet!\n";
    exit(1);    
  }

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  Range Rr(0,domainDimension-2);
  Range Rd(0,domainDimension-1);
  Range R(0,2), D(0,2), R0(base,bound);  // *** we should be able to take D(0,1) but basicInverse ? **
  int axis,dir;
  int revAxis[3] =   { revAxis1,revAxis2,revAxis3 };

  realArray y(R0,R), a(R0,R), b(R0,R), c(R0,R), theta(R0), ct(R0), st(R0);
  
  if( revolutionary->getRangeDimension()<3 )
    y(I,axis3)=0.;
  if( revAxis1==0 && (domainDimension==2 || revAxis2==1) )
  {
    if( computeMap && !computeMapDerivative )
      revolutionary->map(r,y);
    else
      revolutionary->map(r,y,xr);
  }
  else
  {
    realArray rr(R0,Rr);
    for( axis=0; axis<domainDimension-1; axis++ )
      rr(R0,axis)=r(R0,revAxis[axis]);
    if( computeMap && !computeMapDerivative )
      revolutionary->map(rr,y);
    else
      revolutionary->map(rr,y,xr);
  }

  real vv[3];
  for( axis=0; axis<rangeDimension; axis++ ) // assume original grid is in x-y plane
  {
    y(I,axis)-=lineOrigin(axis);
    vv[axis]=lineTangent(axis)*signForTangent;
  }
  b(I,0)=y(I,0)*vv[0]+y(I,1)*vv[1]+y(I,2)*vv[2];

  for( axis=0; axis<rangeDimension; axis++ )
    a(I,axis)=b(I,0)*vv[axis];

  b(I,R)=y(I,R)-a(I,R);

  c(I,0)=vv[1]*b(I,2)-vv[2]*b(I,1);    // c = tangent X b
  c(I,1)=vv[2]*b(I,0)-vv[0]*b(I,2);
  c(I,2)=vv[0]*b(I,1)-vv[1]*b(I,0);

  real dtheta=(endAngle-startAngle)*twoPi;
  theta=dtheta*r(I,revAxis3)+startAngle*twoPi;
  ct=cos(theta);
  st=sin(theta);

  if( computeMap )
  {
    for( axis=0; axis<rangeDimension; axis++ )
      x(I,axis)=a(I,axis)+ (ct*b(I,axis)+st*c(I,axis)) +lineOrigin(axis);
  }
    
  if( computeMapDerivative )
  {
//     for( axis=0; axis<rangeDimension; axis++ )
//       for( dir=0; dir<domainDimension-1; dir++ )
// 	ar(I,axis,dir)=(xr(I,0,dir)*vv[0]+xr(I,1,dir)*vv[1])*(lineTangent(axis)*signForTangent);
    // *wdh* 010228
    realArray ar(R0,R,D), br(R0,R,D), cr(R0,R,D);

    if( revolutionary->getRangeDimension()<3 )
      xr(I,2,Rd)=0.; // AP changed R -> Rd
    for( axis=0; axis<rangeDimension; axis++ )
      for( dir=0; dir<domainDimension-1; dir++ )
	ar(I,axis,dir)=xr(I,0,dir)*(vv[0]*vv[axis])+xr(I,1,dir)*(vv[1]*vv[axis])+xr(I,2,dir)*(vv[2]*vv[axis]);
    

    xr(I,R,domainDimension-1)=0.; // added 980709   *** fix this ****
    ar(I,R,domainDimension-1)=0.;
    br(I,R,Rd)=xr(I,R,Rd)-ar(I,R,Rd);
  
    cr(I,0,Rd)=vv[1]*br(I,2,Rd)-vv[2]*br(I,1,Rd);
    cr(I,1,Rd)=vv[2]*br(I,0,Rd)-vv[0]*br(I,2,Rd);
    cr(I,2,Rd)=vv[0]*br(I,1,Rd)-vv[1]*br(I,0,Rd);
    for( axis=0; axis<3; axis++ )
    {
      for( dir=0; dir<domainDimension-1; dir++ )
        xr(I,axis,revAxis[dir])=ar(I,axis,dir)+(ct*br(I,axis,dir)+st*cr(I,axis,dir));
    }

    const real eps = SQRT(REAL_EPSILON)*.1;
    realArray & sinPhi = theta;   // stor sin(phi) in theta
    switch (params.coordinateType)
    {
    case cartesian:  // mapping returned in cartesian form
    
      for( axis=0; axis<rangeDimension; axis++ )
	xr(I,axis,revAxis3)=(-st*b(I,axis)+ct*c(I,axis))*dtheta;       
      break;
    case spherical: // Mapping returned in spherical form : (phi,theta,r) 
      // derivatives: ( d/d(phi), (1/sin(phi))d/d(theta), d/d(r) )
      // assert( revAxis3==(domainDimension-1) );
      sinPhi(I)=sin(Pi*r(I,revAxis1));   // sin(phi)
      where( fabs(sinPhi(I)) > eps )
      {
	for( axis=0; axis<rangeDimension; axis++ )
	  xr(I,axis,revAxis3)=(-st*b(I,axis)+ct*c(I,axis))*dtheta/sinPhi(I);       
      }
      elsewhere(r(I,revAxis1)<.5 )
      { // phi is near 0.
        // b/sin(phi) = (b/phi) * (phi/sin(phi)) \approx  b_phi 
	for( axis=0; axis<rangeDimension; axis++ )
          xr(I,axis,revAxis3)=(-st*br(I,axis,revAxis1)+ct*cr(I,axis,revAxis1))*(dtheta/Pi);       
      }
      otherwise()
      { // phi is near 1.
        // b/sin(phi) = - (b(pi)-b(phi))/(pi-phi)) *( (pi-phi)/sin(phi) )
	for( axis=0; axis<rangeDimension; axis++ )
          xr(I,axis,revAxis3)=(-st*br(I,axis,revAxis1)+ct*cr(I,axis,revAxis1))*(-dtheta/Pi); // minus sign
      }
      
      break;
    default:
      cout << "RevolutionMapping::map: ERROR not implemented for coordinateType = " 
	<< params.coordinateType << endl;
      {throw "error";}
    }
  }
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//
//   To invert the relation:
//           x(r) = a + R(b) = a + cos(theta)*bb + sin(theta)*cc + x0
//
//   Let 
//      y=x-x0,  a=(v,y)v,  b=y-a  c=vXb  (or c= - vXb : make c2<0)
//   Then
//      da_i/dx_j = v_j*v_i, db_i/dx_j= delta_ij - da_i/dx_j dc_i/dx_j=v X db_i/dx_j
//
//   and       w := a + cos(theta)*b - sin(theta)*c + x0
//   choose theta so that w=(w0,w1,w2)=(w1,w2,0) lies in the x-y plane
//   
//                  a2 + cos(theta)*b2 -sin(theta)*c2 + x02=0
//  Assume that a2+x02=0 then
//                tan(theta)=b2/c2
//  
//        d(theta)/dx_i = (cos()* db3/dx_i - sin()*dc3/dx_i) / (sin()*b3 + cos()*c3 )
//  Invert
//     (w1,w2) -> (r1,r2) and dr_i/dw_j i=0,1 j=0,1
//
// Then
//     dr_i/dx_j = sum_k dw_k/dx_j * dr_i/dw_k
//  where
//     dw_i/dx_j = da_i/dx_j +  cos()*db_i/dx_j - sin()*dc_i/dx_j 
//                             - b_i*sin()d(theta)/d_j -c_i*cos()*d(theta)/dx_j
//
//=================================================================================
void RevolutionMapping::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{
  // printf("RevolutionMapping::basicInverse called\n");
  
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  Range Rr(0,domainDimension-2);
  Range R(0,2), D(0,2), R0(base,bound);  
  int revAxis[3] =   { revAxis1,revAxis2,revAxis3 };

  realArray y(R0,R), a(R0,R), b(R0,R), c(R0,R), theta(R0), ct(R0), st(R0);
  realArray tr(R0,R), wr(R0,R,R), ry(R0,R,R);
    
  int axis,dir;
  real vv[3];
  for( axis=0; axis<rangeDimension; axis++ )
  {
    y(I,axis)=x(I,axis)-lineOrigin(axis);
    vv[axis]=lineTangent(axis)*signForTangent;
  }

  // a = [ (x-x0).v ] v

  b(I,0)=y(I,0)*vv[0]+y(I,1)*vv[1]+y(I,2)*vv[2];
  for( axis=0; axis<rangeDimension; axis++ )
    a(I,axis)=b(I,0)*vv[axis];

  b(I,R)=y(I,R)-a(I,R);
  c(I,0)=vv[1]*b(I,2)-vv[2]*b(I,1);    // c = tangent X b
  c(I,1)=vv[2]*b(I,0)-vv[0]*b(I,2);
  c(I,2)=vv[0]*b(I,1)-vv[1]*b(I,0);

//  bool changeSignOfC = c(2,base) > 0.;
//  if( changeSignOfC )
//    c(R,I)=-c(R,I);             // c= b X tangent
    
  // Solve for theta from
  //   cos(t)*b(axis3) - sin(t)*c(axis3) + a(axis3) +lineOrigin(axis3) = 0
  // ***** assume that a(axis3,I)+lineOrigin(axis3)==0 *********************
  if( vv[2]!=0. || lineOrigin(axis3)!=0. )
  {
    cout << "RevolutionMapping::ERROR in basicInverse: not implemented for lineTangent(axis3)!=0"
      " or lineOrigin(axis3)!=0 \n";
    return;
  }

  real dthetaInverse=1./( (endAngle-startAngle)*twoPi );

  theta=atan2(evaluate(b(I,axis3)),evaluate(c(I,axis3)))+Pi;

  const real theta0=twoPi*startAngle;
  if( computeMap )
  { // shift (theta-startAngle*twoPi) into the interval [0,2Pi]
    if( getIsPeriodic(revAxis3) )
    {
      r(I,revAxis3)=fmod(theta+(twoPi-theta0),twoPi)*dthetaInverse;
    }
    else
    {
      // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
      // delta is the gap in the angle between the start and end of the annulus
      // **NOTE** +theta : result in [0,2pi]
      const real theta1=twoPi*endAngle;
      r(I,revAxis3)=theta;
      real delta = (1.-(endAngle-startAngle))*Pi;
      where ( r(I,revAxis3) < theta0 - delta )
      {
	r(I,revAxis3)+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
      }
      elsewhere ( r(I,revAxis3) > theta1 + delta )
      {
	r(I,revAxis3)-=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
      }
      r(I,revAxis3)=(r(I,revAxis3)-theta0)*dthetaInverse;

    }
  }

  ct=cos(theta);
  st=sin(theta);

  // rotate the vector back into the x-y plane
  
  for( axis=0; axis<rangeDimension; axis++ )
    y(I,axis)=a(I,axis) + (ct*b(I,axis)-st*c(I,axis)) +lineOrigin(axis);
    
  // now compute r(0:1,I) 
  int coordinateType=params.coordinateType;  // save old value
  params.coordinateType=cartesian;  // set to cartesian for the next calls

  realArray rr(R0,R); rr=-1.;
  if( computeMap && !computeMapDerivative )
    if( revolutionary->getBasicInverseOption()==canInvert )
      revolutionary->basicInverse(y,rr,Overture::nullRealDistributedArray(),params);    
    else
      revolutionary->inverseMap(y,rr,Overture::nullRealDistributedArray(),params);    
  else if( computeMap && computeMapDerivative )
    if( revolutionary->getBasicInverseOption()==canInvert )
      revolutionary->basicInverse(y,rr,ry,params);    
    else
      revolutionary->inverseMap(y,rr,ry,params);    
  else 
    if( revolutionary->getBasicInverseOption()==canInvert )
      revolutionary->basicInverse(y,Overture::nullRealDistributedArray(),ry,params);    
    else
      revolutionary->inverseMap(y,Overture::nullRealDistributedArray(),ry,params);    

  params.coordinateType=coordinateType; // reset value

  for( dir=0; dir<domainDimension-1; dir++ )
    r(R0,revAxis[dir])=rr(R0,dir);

  if( false )
  {
    for( int i=base; i<=bound; i++ )
    {
      if( r(i,0)==Mapping::bogus )
      {
	printf("RevolutionMapping:basicInverse:ERROR: revolutionary inverse returned a bogus result! "
	       "This will not work with findNearestGridPoint.\n");
	OV_ABORT("error");
      }
    }
  }
  

  // kkc adjust for coordinate singularites on surfaces
  if ( domainDimension==2 && false )
    { // kkc don't do this because the assumption on the use of lineTangent is wrong! (apparently)
      realArray rs(1,1),xs(1,3);
      for ( int side=0; side<=1; side++ )
	{
	  if ( getTypeOfCoordinateSingularity(side,revAxis1)==polarSingularity )
	    {
	      //	      	      cout<<"adjusting inverse for polar singularity"<<endl;
	      rs=(real)side;
	      revolutionary->map(rs,xs);
	      if ( revolutionary->getRangeDimension()==2 )
		xs(0,2) = 0;

	      Range AXES(rangeDimension);
	      for ( int i=I.getBase(); i<=I.getBound(); i++ )
		{
		  if ( (side==0 && ( (x(i,0)-xs(0,0))*lineTangent(0) + (x(i,1)-xs(0,1))*lineTangent(1) + (x(i,2)-xs(0,2))*lineTangent(2))<100*REAL_MIN )||
		       (side==1 && ( (x(i,0)-xs(0,0))*lineTangent(0) + (x(i,1)-xs(0,1))*lineTangent(1) + (x(i,2)-xs(0,2))*lineTangent(2))>-100*REAL_MIN ) )
		    {
		      r(i,revAxis3) = 0.5;
		      r(i,revAxis1) = (real)side;
		    }
		}
	    }
	}
    }

  if( computeMapDerivative )
  {
    realArray ar(R0,R,D), br(R0,R,D), cr(R0,R,D);

    for( axis=0; axis<3; axis++ )
      for( dir=0; dir<3; dir++ )
      {
        ar(I,axis,dir)=lineTangent(axis)*lineTangent(dir);
        if( axis==dir )
          br(I,axis,dir)= 1.-ar(I,axis,dir);
        else
          br(I,axis,dir)=  -ar(I,axis,dir);
      }

    cr(I,0,R)=vv[1]*br(I,2,R)-vv[2]*br(I,1,R);
    cr(I,1,R)=vv[2]*br(I,0,R)-vv[0]*br(I,2,R);
    cr(I,2,R)=vv[0]*br(I,1,R)-vv[1]*br(I,0,R);
  
    // d(theta)/dx_i
    for( axis=0; axis<rangeDimension; axis++ )
      tr(I,axis)=(ct*br(I,axis3,axis)-st*cr(I,axis3,axis))/(st*b(I,axis3)+ct*c(I,axis3));
    
    for( axis=0; axis<rangeDimension; axis++ )
    {
      // dw_i/dx_j :
      for( dir=0; dir<rangeDimension; dir++ )
        wr(I,axis,dir)=ar(I,axis,dir) + 
	  ct*br(I,axis,dir)-st*cr(I,axis,dir)
	  -st*b(I,axis)*tr(I,dir)-ct*c(I,axis)*tr(I,dir);
    }
    
    for( dir=0; dir<rangeDimension; dir++ )
    {
      for( axis=0; axis<domainDimension-1; axis++ )
	rx(I,revAxis[axis],dir)=wr(I,0,dir)*ry(I,axis,0)+wr(I,1,dir)*ry(I,axis,1);
    }
    
    const real eps = SQRT(REAL_EPSILON)*.1;
    realArray & sinPhi = theta;   // stor sin(phi) in theta
    switch (params.coordinateType)
    {
    case cartesian:  // mapping returned in cartesian form
    
      for( dir=0; dir<rangeDimension; dir++ )
	rx(I,revAxis3,dir)=tr(I,dir)*dthetaInverse; 
      break;
    case spherical: // Mapping returned in spherical form : (phi,theta,r) 
      // derivatives: ( d(phi)/d(x) sin(phi)*d(theta)/dx dr/dx)

      // assert( revAxis3==(domainDimension-1) );
      sinPhi(I)=sin(Pi*r(I,revAxis1));   // sin(phi)
      where( fabs(sinPhi(I)) > eps )
      {
	for( dir=0; dir<rangeDimension; dir++ )
	  rx(I,revAxis3,dir)=tr(I,dir)*dthetaInverse*sinPhi(I); 
      }
      elsewhere(r(I,revAxis1)<.5)
      {
        //  b/sin(phi) = (b/phi) * (phi/sin(phi)) \approx  b_phi 
        // tr*sin(phi) --> b -> d(b)/d(phi)  , c-> d(c)/d(phi)
	for( dir=0; dir<rangeDimension; dir++ )
	  rx(I,revAxis3,dir)=(dthetaInverse*Pi)*
	    (ct*br(I,axis3,dir)-st*cr(I,axis3,dir))/
                   (st*br(I,axis3,revAxis1)+ct*cr(I,axis3,revAxis1));
      }
      otherwise()
      {
        // b(phi)/(pi-phi) = -(b(pi)-b(phi))/(pi-phi)
	for( dir=0; dir<rangeDimension; dir++ )
	  rx(I,revAxis3,dir)=(-dthetaInverse*Pi)*                   // note minus sign
                 (ct*br(I,axis3,dir)-st*cr(I,axis3,dir))/
                   (st*br(I,axis3,revAxis1)+ct*cr(I,axis3,revAxis1));
      }
      
      break;
    default:
      cout << "RevolutionMapping::basicInverse: ERROR not implemented for coordinateType = " 
	<< params.coordinateType << endl;
      OV_ABORT("error");
    }



  }
}


// &&&&&&&&&&&&&&&&&

void RevolutionMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
// =========================================================================================
//   new *serial-array version*
// =========================================================================================
{
  if( revolutionary==NULL )
  {
    cout << "RevolutionMapping::map: Error: The mapping to be revolved has not been defined yet!\n";
    exit(1);    
  }

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  Range Rr(0,domainDimension-2);
  Range Rd(0,domainDimension-1);
  Range R(0,2), D(0,2), R0(base,bound);  // *** we should be able to take D(0,1) but basicInverse ? **
  int axis,dir;
  int revAxis[3] =   { revAxis1,revAxis2,revAxis3 };

  RealArray y(R0,R), a(R0,R), b(R0,R), c(R0,R), theta(R0), ct(R0), st(R0);
  
  if( revolutionary->getRangeDimension()<3 )
    y(I,axis3)=0.;
  if( revAxis1==0 && (domainDimension==2 || revAxis2==1) )
  {
    if( computeMap && !computeMapDerivative )
      revolutionary->mapS(r,y);
    else
      revolutionary->mapS(r,y,xr);
  }
  else
  {
    RealArray rr(R0,Rr);
    for( axis=0; axis<domainDimension-1; axis++ )
      rr(R0,axis)=r(R0,revAxis[axis]);
    if( computeMap && !computeMapDerivative )
      revolutionary->mapS(rr,y);
    else
      revolutionary->mapS(rr,y,xr);
  }

  real vv[3];
  for( axis=0; axis<rangeDimension; axis++ ) // assume original grid is in x-y plane
  {
    y(I,axis)-=lineOrigin(axis);
    vv[axis]=lineTangent(axis)*signForTangent;
  }
  b(I,0)=y(I,0)*vv[0]+y(I,1)*vv[1]+y(I,2)*vv[2];

  for( axis=0; axis<rangeDimension; axis++ )
    a(I,axis)=b(I,0)*vv[axis];

  b(I,R)=y(I,R)-a(I,R);

  c(I,0)=vv[1]*b(I,2)-vv[2]*b(I,1);    // c = tangent X b
  c(I,1)=vv[2]*b(I,0)-vv[0]*b(I,2);
  c(I,2)=vv[0]*b(I,1)-vv[1]*b(I,0);

  real dtheta=(endAngle-startAngle)*twoPi;
  theta=dtheta*r(I,revAxis3)+startAngle*twoPi;
  ct=cos(theta);
  st=sin(theta);

  if( computeMap )
  {
    for( axis=0; axis<rangeDimension; axis++ )
      x(I,axis)=a(I,axis)+ (ct*b(I,axis)+st*c(I,axis)) +lineOrigin(axis);
  }
    
  if( computeMapDerivative )
  {
//     for( axis=0; axis<rangeDimension; axis++ )
//       for( dir=0; dir<domainDimension-1; dir++ )
// 	ar(I,axis,dir)=(xr(I,0,dir)*vv[0]+xr(I,1,dir)*vv[1])*(lineTangent(axis)*signForTangent);
    // *wdh* 010228
    RealArray ar(R0,R,D), br(R0,R,D), cr(R0,R,D);

    if( revolutionary->getRangeDimension()<3 )
      xr(I,2,Rd)=0.; // AP changed R -> Rd
    for( axis=0; axis<rangeDimension; axis++ )
      for( dir=0; dir<domainDimension-1; dir++ )
	ar(I,axis,dir)=xr(I,0,dir)*(vv[0]*vv[axis])+xr(I,1,dir)*(vv[1]*vv[axis])+xr(I,2,dir)*(vv[2]*vv[axis]);
    

    xr(I,R,domainDimension-1)=0.; // added 980709   *** fix this ****
    ar(I,R,domainDimension-1)=0.;
    br(I,R,Rd)=xr(I,R,Rd)-ar(I,R,Rd);
  
    cr(I,0,Rd)=vv[1]*br(I,2,Rd)-vv[2]*br(I,1,Rd);
    cr(I,1,Rd)=vv[2]*br(I,0,Rd)-vv[0]*br(I,2,Rd);
    cr(I,2,Rd)=vv[0]*br(I,1,Rd)-vv[1]*br(I,0,Rd);
    for( axis=0; axis<3; axis++ )
    {
      for( dir=0; dir<domainDimension-1; dir++ )
        xr(I,axis,revAxis[dir])=ar(I,axis,dir)+(ct*br(I,axis,dir)+st*cr(I,axis,dir));
    }

    const real eps = SQRT(REAL_EPSILON)*.1;
    RealArray & sinPhi = theta;   // stor sin(phi) in theta
    switch (params.coordinateType)
    {
    case cartesian:  // mapping returned in cartesian form
    
      for( axis=0; axis<rangeDimension; axis++ )
	xr(I,axis,revAxis3)=(-st*b(I,axis)+ct*c(I,axis))*dtheta;       
      break;
    case spherical: // Mapping returned in spherical form : (phi,theta,r) 
      // derivatives: ( d/d(phi), (1/sin(phi))d/d(theta), d/d(r) )
      // assert( revAxis3==(domainDimension-1) );
      sinPhi(I)=sin(Pi*r(I,revAxis1));   // sin(phi)
      where( fabs(sinPhi(I)) > eps )
      {
	for( axis=0; axis<rangeDimension; axis++ )
	  xr(I,axis,revAxis3)=(-st*b(I,axis)+ct*c(I,axis))*dtheta/sinPhi(I);       
      }
      elsewhere(r(I,revAxis1)<.5 )
      { // phi is near 0.
        // b/sin(phi) = (b/phi) * (phi/sin(phi)) \approx  b_phi 
	for( axis=0; axis<rangeDimension; axis++ )
          xr(I,axis,revAxis3)=(-st*br(I,axis,revAxis1)+ct*cr(I,axis,revAxis1))*(dtheta/Pi);       
      }
      otherwise()
      { // phi is near 1.
        // b/sin(phi) = - (b(pi)-b(phi))/(pi-phi)) *( (pi-phi)/sin(phi) )
	for( axis=0; axis<rangeDimension; axis++ )
          xr(I,axis,revAxis3)=(-st*br(I,axis,revAxis1)+ct*cr(I,axis,revAxis1))*(-dtheta/Pi); // minus sign
      }
      
      break;
    default:
      cout << "RevolutionMapping::map: ERROR not implemented for coordinateType = " 
	<< params.coordinateType << endl;
      {throw "error";}
    }
  }
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//
//   To invert the relation:
//           x(r) = a + R(b) = a + cos(theta)*bb + sin(theta)*cc + x0
//
//   Let 
//      y=x-x0,  a=(v,y)v,  b=y-a  c=vXb  (or c= - vXb : make c2<0)
//   Then
//      da_i/dx_j = v_j*v_i, db_i/dx_j= delta_ij - da_i/dx_j dc_i/dx_j=v X db_i/dx_j
//
//   and       w := a + cos(theta)*b - sin(theta)*c + x0
//   choose theta so that w=(w0,w1,w2)=(w1,w2,0) lies in the x-y plane
//   
//                  a2 + cos(theta)*b2 -sin(theta)*c2 + x02=0
//  Assume that a2+x02=0 then
//                tan(theta)=b2/c2
//  
//        d(theta)/dx_i = (cos()* db3/dx_i - sin()*dc3/dx_i) / (sin()*b3 + cos()*c3 )
//  Invert
//     (w1,w2) -> (r1,r2) and dr_i/dw_j i=0,1 j=0,1
//
// Then
//     dr_i/dx_j = sum_k dw_k/dx_j * dr_i/dw_k
//  where
//     dw_i/dx_j = da_i/dx_j +  cos()*db_i/dx_j - sin()*dc_i/dx_j 
//                             - b_i*sin()d(theta)/d_j -c_i*cos()*d(theta)/dx_j
//
//=================================================================================
void RevolutionMapping::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
{
  // printf("RevolutionMapping::basicInverse called\n");
  
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  Range Rr(0,domainDimension-2);
  Range R(0,2), D(0,2), R0(base,bound);  
  int revAxis[3] =   { revAxis1,revAxis2,revAxis3 };

  RealArray y(R0,R), a(R0,R), b(R0,R), c(R0,R), theta(R0), ct(R0), st(R0);
  RealArray tr(R0,R), wr(R0,R,R), ry(R0,R,R);
    
  int axis,dir;
  real vv[3];
  for( axis=0; axis<rangeDimension; axis++ )
  {
    y(I,axis)=x(I,axis)-lineOrigin(axis);
    vv[axis]=lineTangent(axis)*signForTangent;
  }

  // a = [ (x-x0).v ] v

  b(I,0)=y(I,0)*vv[0]+y(I,1)*vv[1]+y(I,2)*vv[2];
  for( axis=0; axis<rangeDimension; axis++ )
    a(I,axis)=b(I,0)*vv[axis];

  b(I,R)=y(I,R)-a(I,R);
  c(I,0)=vv[1]*b(I,2)-vv[2]*b(I,1);    // c = tangent X b
  c(I,1)=vv[2]*b(I,0)-vv[0]*b(I,2);
  c(I,2)=vv[0]*b(I,1)-vv[1]*b(I,0);

//  bool changeSignOfC = c(2,base) > 0.;
//  if( changeSignOfC )
//    c(R,I)=-c(R,I);             // c= b X tangent
    
  // Solve for theta from
  //   cos(t)*b(axis3) - sin(t)*c(axis3) + a(axis3) +lineOrigin(axis3) = 0
  // ***** assume that a(axis3,I)+lineOrigin(axis3)==0 *********************
  if( vv[2]!=0. || lineOrigin(axis3)!=0. )
  {
    cout << "RevolutionMapping::ERROR in basicInverse: not implemented for lineTangent(axis3)!=0"
      " or lineOrigin(axis3)!=0 \n";
    return;
  }

  real dthetaInverse=1./( (endAngle-startAngle)*twoPi );

  theta=atan2(evaluate(b(I,axis3)),evaluate(c(I,axis3)))+Pi;

  const real theta0=twoPi*startAngle;
  if( computeMap )
  { // shift (theta-startAngle*twoPi) into the interval [0,2Pi]
    if( getIsPeriodic(revAxis3) )
    {
      r(I,revAxis3)=fmod(theta+(twoPi-theta0),twoPi)*dthetaInverse;
    }
    else
    {
      // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
      // delta is the gap in the angle between the start and end of the annulus
      // **NOTE** +theta : result in [0,2pi]
      const real theta1=twoPi*endAngle;
      r(I,revAxis3)=theta;
      real delta = (1.-(endAngle-startAngle))*Pi;
      where ( r(I,revAxis3) < theta0 - delta )
      {
	r(I,revAxis3)+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
      }
      elsewhere ( r(I,revAxis3) > theta1 + delta )
      {
	r(I,revAxis3)-=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
      }
      r(I,revAxis3)=(r(I,revAxis3)-theta0)*dthetaInverse;

    }
  }

  ct=cos(theta);
  st=sin(theta);

  // rotate the vector back into the x-y plane
  
  for( axis=0; axis<rangeDimension; axis++ )
    y(I,axis)=a(I,axis) + (ct*b(I,axis)-st*c(I,axis)) +lineOrigin(axis);
    
  // now compute r(0:1,I) 
  int coordinateType=params.coordinateType;  // save old value
  params.coordinateType=cartesian;  // set to cartesian for the next calls

  RealArray rr(R0,R); rr=-1.;
  if( computeMap && !computeMapDerivative )
    if( revolutionary->getBasicInverseOption()==canInvert )
      revolutionary->basicInverseS(y,rr,Overture::nullRealArray(),params);    
    else
      revolutionary->inverseMapS(y,rr,Overture::nullRealArray(),params);    
  else if( computeMap && computeMapDerivative )
    if( revolutionary->getBasicInverseOption()==canInvert )
      revolutionary->basicInverseS(y,rr,ry,params);    
    else
      revolutionary->inverseMapS(y,rr,ry,params);    
  else 
    if( revolutionary->getBasicInverseOption()==canInvert )
      revolutionary->basicInverseS(y,Overture::nullRealArray(),ry,params);    
    else
      revolutionary->inverseMapS(y,Overture::nullRealArray(),ry,params);    

  params.coordinateType=coordinateType; // reset value

  for( dir=0; dir<domainDimension-1; dir++ )
    r(R0,revAxis[dir])=rr(R0,dir);

  // kkc adjust for coordinate singularites on surfaces
  if ( domainDimension==2 && false )
    { // kkc don't do this because the assumption on the use of lineTangent is wrong! (apparently)
      RealArray rs(1,1),xs(1,3);
      for ( int side=0; side<=1; side++ )
	{
	  if ( getTypeOfCoordinateSingularity(side,revAxis1)==polarSingularity )
	    {
	      //	      	      cout<<"adjusting inverse for polar singularity"<<endl;
	      rs=(real)side;
	      revolutionary->mapS(rs,xs);
	      if ( revolutionary->getRangeDimension()==2 )
		xs(0,2) = 0;

	      Range AXES(rangeDimension);
	      for ( int i=I.getBase(); i<=I.getBound(); i++ )
		{
		  if ( (side==0 && ( (x(i,0)-xs(0,0))*lineTangent(0) + (x(i,1)-xs(0,1))*lineTangent(1) + (x(i,2)-xs(0,2))*lineTangent(2))<100*REAL_MIN )||
		       (side==1 && ( (x(i,0)-xs(0,0))*lineTangent(0) + (x(i,1)-xs(0,1))*lineTangent(1) + (x(i,2)-xs(0,2))*lineTangent(2))>-100*REAL_MIN ) )
		    {
		      r(i,revAxis3) = 0.5;
		      r(i,revAxis1) = (real)side;
		    }
		}
	    }
	}
    }

  if( computeMapDerivative )
  {
    RealArray ar(R0,R,D), br(R0,R,D), cr(R0,R,D);

    for( axis=0; axis<3; axis++ )
      for( dir=0; dir<3; dir++ )
      {
        ar(I,axis,dir)=lineTangent(axis)*lineTangent(dir);
        if( axis==dir )
          br(I,axis,dir)= 1.-ar(I,axis,dir);
        else
          br(I,axis,dir)=  -ar(I,axis,dir);
      }

    cr(I,0,R)=vv[1]*br(I,2,R)-vv[2]*br(I,1,R);
    cr(I,1,R)=vv[2]*br(I,0,R)-vv[0]*br(I,2,R);
    cr(I,2,R)=vv[0]*br(I,1,R)-vv[1]*br(I,0,R);
  
    // d(theta)/dx_i
    for( axis=0; axis<rangeDimension; axis++ )
      tr(I,axis)=(ct*br(I,axis3,axis)-st*cr(I,axis3,axis))/(st*b(I,axis3)+ct*c(I,axis3));
    
    for( axis=0; axis<rangeDimension; axis++ )
    {
      // dw_i/dx_j :
      for( dir=0; dir<rangeDimension; dir++ )
        wr(I,axis,dir)=ar(I,axis,dir) + 
	  ct*br(I,axis,dir)-st*cr(I,axis,dir)
	  -st*b(I,axis)*tr(I,dir)-ct*c(I,axis)*tr(I,dir);
    }
    
    for( dir=0; dir<rangeDimension; dir++ )
    {
      for( axis=0; axis<domainDimension-1; axis++ )
	rx(I,revAxis[axis],dir)=wr(I,0,dir)*ry(I,axis,0)+wr(I,1,dir)*ry(I,axis,1);
    }
    
    const real eps = SQRT(REAL_EPSILON)*.1;
    RealArray & sinPhi = theta;   // stor sin(phi) in theta
    switch (params.coordinateType)
    {
    case cartesian:  // mapping returned in cartesian form
    
      for( dir=0; dir<rangeDimension; dir++ )
	rx(I,revAxis3,dir)=tr(I,dir)*dthetaInverse; 
      break;
    case spherical: // Mapping returned in spherical form : (phi,theta,r) 
      // derivatives: ( d(phi)/d(x) sin(phi)*d(theta)/dx dr/dx)

      // assert( revAxis3==(domainDimension-1) );
      sinPhi(I)=sin(Pi*r(I,revAxis1));   // sin(phi)
      where( fabs(sinPhi(I)) > eps )
      {
	for( dir=0; dir<rangeDimension; dir++ )
	  rx(I,revAxis3,dir)=tr(I,dir)*dthetaInverse*sinPhi(I); 
      }
      elsewhere(r(I,revAxis1)<.5)
      {
        //  b/sin(phi) = (b/phi) * (phi/sin(phi)) \approx  b_phi 
        // tr*sin(phi) --> b -> d(b)/d(phi)  , c-> d(c)/d(phi)
	for( dir=0; dir<rangeDimension; dir++ )
	  rx(I,revAxis3,dir)=(dthetaInverse*Pi)*
	    (ct*br(I,axis3,dir)-st*cr(I,axis3,dir))/
                   (st*br(I,axis3,revAxis1)+ct*cr(I,axis3,revAxis1));
      }
      otherwise()
      {
        // b(phi)/(pi-phi) = -(b(pi)-b(phi))/(pi-phi)
	for( dir=0; dir<rangeDimension; dir++ )
	  rx(I,revAxis3,dir)=(-dthetaInverse*Pi)*                   // note minus sign
                 (ct*br(I,axis3,dir)-st*cr(I,axis3,dir))/
                   (st*br(I,axis3,revAxis1)+ct*cr(I,axis3,revAxis1));
      }
      
      break;
    default:
      cout << "RevolutionMapping::basicInverse: ERROR not implemented for coordinateType = " 
	<< params.coordinateType << endl;
      {throw "error";}
    }



  }
}






//=================================================================================
// get a mapping from the database
//=================================================================================
int RevolutionMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering RevolutionMapping::get" << endl;

  subDir.get( RevolutionMapping::className,"className" ); 
  if( RevolutionMapping::className != "RevolutionMapping" )
  {
    cout << "RevolutionMapping::get ERROR in className!" << endl;
  }
  subDir.get(startAngle,"startAngle");
  subDir.get(endAngle,"endAngle");
  subDir.get(lineOrigin,"lineOrigin");
  subDir.get(lineTangent,"lineTangent");
  subDir.get(revAxis1,"revAxis1");
  subDir.get(revAxis2,"revAxis2");
  subDir.get(revAxis3,"revAxis3");
  subDir.get(signForTangent,"signForTangent");
  subDir.get(uninitialized,"uninitialized");

  aString mappingClassName;
  subDir.get(mappingClassName,"revolutionary.className");  
  revolutionary = Mapping::makeMapping( mappingClassName );  // ***** this does a new -- who will delete? ***
  if( revolutionary==NULL )
  {
    cout << "RevolutionMapping::get:ERROR unable to make the mapping with className = " 
      << mappingClassName << endl;
    return 1;
  }
  revolutionary->incrementReferenceCount();
  revolutionary->get( subDir,"revolutionary" ); 
  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  mappingHasChanged();
  return 0;
}

int RevolutionMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( RevolutionMapping::className,"className" );
  subDir.put(startAngle,"startAngle");
  subDir.put(endAngle,"endAngle");
  subDir.put(lineOrigin,"lineOrigin");
  subDir.put(lineTangent,"lineTangent");
  subDir.put(revAxis1,"revAxis1");
  subDir.put(revAxis2,"revAxis2");
  subDir.put(revAxis3,"revAxis3");
  subDir.put(signForTangent,"signForTangent");
  subDir.put(uninitialized,"uninitialized");

  subDir.put( revolutionary->getClassName(),"revolutionary.className"); // save the class name so we can do a 
  // "makeMapping" in the get function
  revolutionary->put( subDir,"revolutionary" );
  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping *RevolutionMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==RevolutionMapping::className )
    retval = new RevolutionMapping();
  return retval;
}

int RevolutionMapping:: 
setRevolutionAngle(const real startAngle_ /* =0. */, 
                   const real endAngle_ /* =1. */ )
//===========================================================================
/// \brief  Define the angle through which the revolution progresses.
/// \param startAngle_ (input) : starting "angle" (in [0,1]) for the revolution.
/// \param endAngle_ (input) : ending "angle" (in [0,1]) for the revolution.
//===========================================================================
{
  startAngle=startAngle_;
  endAngle=endAngle_;
  if( fabs(endAngle-startAngle-1.)<REAL_EPSILON*10. )
  { 
    setIsPeriodic(revAxis3,functionPeriodic );  
    setBoundaryCondition( Start,revAxis3,-1 );
    setBoundaryCondition(   End,revAxis3,-1 );
  }	
  else
  {
    if( getIsPeriodic(revAxis3)==functionPeriodic )
    {
      setIsPeriodic(revAxis3,notPeriodic );  
      setBoundaryCondition(Start,revAxis3,3);
      setBoundaryCondition(  End,revAxis3,4);
    }
  }
  mappingHasChanged();
  return 0;
}

int RevolutionMapping:: 
getRevolutionAngle( real & startAngle_, 
		    real & endAngle_  )
//===========================================================================
/// \brief  Get the bounding angles in the revolution progresses.
/// \param startAngle_ (input) : starting "angle" for the revolution.
/// \param endAngle_ (input) : ending "angle" for the revolution.
//===========================================================================
{
  startAngle_=startAngle;
  endAngle_=endAngle;
  return 0;
}

    
int RevolutionMapping:: 
setParameterAxes( const int & revAxis1_, const int & revAxis2_, const int & revAxis3_ )
//===========================================================================
/// \brief  Define the parameter axes the mapping. The 2D mapping will
///    be evaluated with {\tt (r(I,revAxis1),\-r(I,revAxis2))} while {\tt r(I,revAxis3)}
///    will correspond to the angle of revolution $\theta$.
///    The choice of these variables is normally only important if the body of revolution
///    has a spherical polar singularity at one or both ends and the user wants to remove
///    the singularity using the orthographic projection.(reparameterization option). The
///    orthographic project expects the mapping to parameterized like a sphere with
///    the parameters in the order $(\phi,\theta,r)$. 
/// 
///   <ul>
///     <li> <B>revAxis1</B> The axis corresponding to $\phi$ in a spherical coordinate systems 
///          or the axial variable $s$ in cylindrical  coordinates. {\tt revAxis1} will
///          normally be $0$ (or $1$) and correspond to the axial like variable in the 2D mapping
///          that is being revolved.
///     <li> <B>revAxis2</B> The axis corresponding to $r$ in a spherical coordinate system. Normally
///           {revAxis2=2} so the axial variable appears last.
///     <li> <B>revAxis3</B> The axis corresponding to $\theta$ in a spherical coordinate system. Normally
///         {revAxis3=1}.
///   </ul>
///    
/// \param revAxis1_,revAxis2_,revAxis3_ (input) : A permutation of (0,1,2).
//===========================================================================
{
  if( revAxis1_==revAxis1 && revAxis2_==revAxis2 && revAxis3_==revAxis3 )
    return 0;  // axes are the same, no changes necessary
  
  if( revAxis1_<0 || revAxis1_>2 || revAxis2_<0 || revAxis2_>2 || revAxis3_<0 || revAxis3_>2 ||
      revAxis1_==revAxis2_ || revAxis1_==revAxis3_ || revAxis2_==revAxis3_ )
  {
    cout << "RevolutionMapping::setAxes:ERROR: invalid values for axes, revAxis1=" << revAxis1_
         << ", revAxis2=" << revAxis2_ << ", revAxis3=" << revAxis3_ << endl;
    cout << " These values should be a permutation of (0,1,2). No changes made \n";
    return 1;
  }
  
  // Save the old values for gridDimension, bc, periodic so we can re-assign
  int side,axis;
  int dimensions[3],boundaryCondition[2][3];
  periodicType periodic[3];
  for( axis=0; axis<domainDimension; axis++ )
  {
    dimensions[axis]=getGridDimensions(axis);
    periodic[axis]=getIsPeriodic(axis);
    for( side=Start; side<=End; side++ )
      boundaryCondition[side][axis]=getBoundaryCondition(side,axis);   
  }
  
  const int revAxisOld[3]= {revAxis1,revAxis2,revAxis3 }; 

  revAxis1=revAxis1_;
  revAxis2=revAxis2_;
  revAxis3=revAxis3_;
  if( domainDimension==2 )
  {
    revAxis1=min(1,revAxis1);
    revAxis3=(revAxis1+1) % 2;
  }
  
  int revAxis[3] = {revAxis1,revAxis2,revAxis3 };
  for( axis=0; axis<domainDimension; axis++ )
  {
    const int axisOld=revAxisOld[axis];
    setGridDimensions(revAxis[axis],dimensions[axisOld]);
    setIsPeriodic(revAxis[axis],periodic[axisOld] );
    for( side=Start; side<=End; side++ )
      setBoundaryCondition(side,revAxis[axis],boundaryCondition[side][axisOld]);
  }
  return 0;
}

int RevolutionMapping:: 
setRevolutionary(Mapping & revolutionary_)
//===========================================================================
/// \brief  Define the mapping that will be revolved.
/// \param revolutionary_input) : mapping to revolve.
//===========================================================================
{
  revolutionary=&revolutionary_;
  revolutionary->incrementReferenceCount();
  
  if( revolutionary->getDomainDimension()==1 && domainDimension==3 )
  {
    revAxis1=min(1,revAxis1);
    revAxis3=(revAxis1+1) % 2;
    setRevolutionAngle(startAngle,endAngle);  // this will set periodicity in revAxis3 direction.
    
  }
  // Define properties of this mapping
  if( revolutionary->getRangeDimension()==2 )
  {
    setBasicInverseOption(canInvert); // we can invert if the revolutionary is in the plane.
  }
  else
  {
    setBasicInverseOption(canDoNothing);
  }
  
  inverseIsDistributed=revolutionary->usesDistributedInverse();  // *wdh* 110629

  
  setDomainDimension(revolutionary->getDomainDimension()+1);
  setRangeDimension(3 /*revolutionary->getRangeDimension()+1*/);
  
  int revAxis[3] =   { revAxis1,revAxis2,revAxis3 };
   
  for( int axis=0; axis<domainDimension-1; axis++ )
  {
    setGridDimensions(revAxis[axis],max(9,revolutionary->getGridDimensions(axis)));
    setIsPeriodic(revAxis[axis],revolutionary->getIsPeriodic(axis));
    for( int side=Start; side<=End; side++ )
    {
      setBoundaryCondition(side,revAxis[axis],revolutionary->getBoundaryCondition(side,axis));
      setShare(side,revAxis[axis],revolutionary->getShare(side,axis));
    }
  }
  // default number of grid points in the theta direction
  setGridDimensions(revAxis3,max(9,int(31*(endAngle-startAngle))));  

  initialize();   // we cannot wait to do this since the properties of the mapping may be queried
  return 0;
}

int RevolutionMapping:: 
setLineOfRevolution(const RealArray & lineOrigin_,
		    const RealArray & lineTangent_ )
//===========================================================================
/// \brief  Define the point of origin and the tangent of the line of revolution. 
///  *For now this point and line must
///     lie in the x-y plane (lineOrigin\_(2)==0, lineTangent\_(2)==0)
/// \param lineOrigin_ (input) : the point of origin for the line of revolution. For
///     now we require lineOrigin\_(2)==0.
/// \param lineTangent_ (input) : the tangent to the line of revolution with lineTangent_(2)==0
//===========================================================================
{
  if( lineOrigin_.getLength(0) >= 3 )
  {
    lineOrigin=lineOrigin_(Range(0,2));
//      if( lineOrigin(2)!=0. )
//        printf("RevolutionMapping::setLineOfRevolution:ERROR: lineOrigin(2) should be zero\n");
//      lineOrigin(2)=0.;  // **** make this restriction
  }
  else
  {
    if( lineOrigin_.getLength(0)>0 )
      printf("RevolutionMapping::setLineOfRevolution:ERROR: lineOrigin should be at least length 3\n");
    lineOrigin.redim(3);
    lineOrigin=0.; 
    lineOrigin(0)=-1.;   // revolve about a line that passes through this point, (-1,0,0)
  }    
  
  if( lineTangent_.getLength(0) >= 3 )
  {
    lineTangent=lineTangent_(Range(0,2));
//      if( lineTangent(2)!=0. )
//        printf("RevolutionMapping::setLineOfRevolution:ERROR: lineTangent(2) should be zero\n");

//      lineTangent(2)=0.;  // **** make this restriction
    // normalize the tangent vector
    real norm = SQRT( SQR(lineTangent(0))+SQR(lineTangent(1))+SQR(lineTangent(2)) );
    if( norm==0. )
    {
      cout << "RevolutionMapping::error: tangent is the zero vector! \n";
      lineTangent(1)=1.;  // reset to default value
    }
    else
    {
      lineTangent(Range(0,2))/=norm;
    }
  }
  else
  {
    if( lineTangent_.getLength(0)>0 )
      printf("RevolutionMapping::setLineOfRevolution:ERROR: lineTangent should be at least length 3\n");
    lineTangent.redim(3);
    lineTangent=0.; lineTangent(1)=1.; // revolve about the y-axis by default
  }
  initialize();   // we cannot wait to do this since the properties of the mapping may be queried
  return 0;
}


//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int RevolutionMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!RevolutionMapping",
      "revolve which mapping?",
      "start/end angle",
      "tangent of line to revolve about",
      "choose a point on the line to revolve about",
      "parameter axes",
      "edit revolutionary",
      "force polar singularity",
      "unset polar singularity",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "check",
      "check inverse",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "revolve which mapping?",
      "start/end angle        : revolve through these angles",
      "tangent of line to revolve about",
      "choose a point on the line to revolve about",
      "parameter axes     : set parameter axes",
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

#define SC (char *)(const char *)

  bool plotObject=TRUE;
  bool mappingChosen = revolutionary!=NULL;
  Mapping *newRevolutionary = NULL;

  // By default transform the last mapping in the list (if this mapping is unitialized, mappingChosen==FALSE)
  if( !mappingChosen )
  {
    int number= mapInfo.mappingList.getLength();
    for( int i=number-1; i>=0; i-- )
    {
      Mapping *mapPointer=mapInfo.mappingList[i].mapPointer;
      const int dd = mapPointer->getDomainDimension();
      const int rd =mapPointer->getRangeDimension();
      if( (dd==1 && (rd==2 || rd==3) ) || (dd==2 && rd==2 ) ) // we can revolve lines in 2d/3d or 2d patch
      {
        newRevolutionary=mapPointer;   // use this one
// AP: debug
//	printf("Name of revolutionary: %s\n", SC mapPointer->getName(mappingName));
	
        mappingHasChanged();
	break; 
      }
    }
  }
  if( revolutionary == NULL && newRevolutionary==NULL )
  {
    cout << "RevolutionMapping:ERROR: there are no mappings that can be revolved!! \n";
    cout << "A revolvable mapping should have domainDimension= 1 or 2 and rangeDimension=2 or 3   \n";
    return 1;
  }


  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("Revolution>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="revolve which mapping?" )
    { // Make a menu with the Mapping names (only maps from R^2 -> R^2)
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int j=0;
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( (map.getDomainDimension()==1 || map.getDomainDimension()==2) && 
	    (map.getRangeDimension()==2  || map.getRangeDimension()==3) )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      menu2[j++]="none"; 
      menu2[j]="";   // null string terminates the menu
      int mapNumber = gi.getMenuItem(menu2,answer2);
      delete [] menu2;
      if( answer2=="none" )
        continue;
      if( mapNumber<0 )
      {
        gi.outputString("Error: unknown mapping to revolve!");
        gi.stopReadingCommandFile();
	break;
      }
      else
      {
        mapNumber=subListNumbering(mapNumber);  // map number in the original list
        if( mapInfo.mappingList[mapNumber].mapPointer==this )
        {
  	  cout << "RevolutionMapping::ERROR: you cannot transform this mapping, this would be recursive!\n";
          continue;
	}
      }
      mappingHasChanged();
      newRevolutionary=mapInfo.mappingList[mapNumber].mapPointer;
    }
    if( (!mappingChosen && it==0) || answer=="revolve which mapping?" )
    {
      // Define properties of this mapping
      assert( newRevolutionary!=NULL );
      if( getName(mappingName)=="revolutionMapping" )
        setName(mappingName,aString("revolution-")+newRevolutionary->getName(mappingName));
      setRevolutionary(*newRevolutionary);
      mappingHasChanged();
      mappingChosen=TRUE;
      plotObject=TRUE;
    }
    else if( answer=="tangent of line to revolve about" ) 
    {
      gi.inputString(line,sPrintF(buff,"Enter the tangent (t0,t1,t2=0) (default=(%e,%e,%e)): ",
				  lineTangent(0),lineTangent(1),lineTangent(2)));
      if( line!="" )
      {
        sScanF(line,"%e %e %e",&lineTangent(0),&lineTangent(1),&lineTangent(2)); // **DON'T restrict t2=0

        setLineOfRevolution(lineOrigin,lineTangent);
        mappingHasChanged();
      }
    }
    else if( answer=="choose a point on the line to revolve about" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the point (x0,y0,z0=0.) (default=(%e,%e,%e)): ",
				  lineOrigin(0),lineOrigin(1),lineOrigin(2)));
      if( line!="" ) 
      {
        sScanF(line,"%e %e %e",&lineOrigin(0),&lineOrigin(1),&lineOrigin(2)); // DON'T restrict lineOrigin(2)==0

        setLineOfRevolution(lineOrigin,lineTangent);
        mappingHasChanged();
      }
    }
    else if( answer=="start/end angle" )
    {
      gi.inputString(line,sPrintF(buff,"Enter starting and ending angle (degress) (default=%e,%e): ",
         startAngle*360.,endAngle*360.));
      if( line!="" ) 
      {
	sScanF( line,"%e %e",&startAngle,&endAngle);
	startAngle/=360.;  // convert to unit interval, can be negative
	endAngle/=360.;
	setRevolutionAngle(startAngle,endAngle);
	mappingHasChanged();
      }
    }
    else if(answer=="parameter axes" )
    {
      gi.inputString(line,sPrintF(buff,
          "Input the parameters axes revAxis1,revAxis2,revAxis3 (a permutation of 0,1,2) (default=%i,%i,%i): ",
          revAxis1,revAxis2,revAxis3));
      int revAxis1_,revAxis2_,revAxis3_;
      if( line!="" )
      {
        sScanF( line,"%i %i %i",&revAxis1_,&revAxis2_,&revAxis3_);
        setParameterAxes(revAxis1_,revAxis2_,revAxis3_);
        mappingHasChanged();
      }
    }
    else if( answer=="force polar singularity" )
    {
      gi.inputString(line,"Enter the side,axis of the face to force a polar singularity (2 integers)");
      int side=-1, axis=-1;
      sScanF(line,"%i %i",&side,&axis);
      if( side>=0 && side<=1 && axis>=0 && axis<domainDimension )
      {
	printF("Setting a polar singularity for (side,axis)=(%i,%i)\n",side,axis);
	if( getTypeOfCoordinateSingularity( side,revAxis1 ) != polarSingularity )
	{
	  if( revAxis1==0 && revAxis2==1 && revAxis3==2 )
	  {
	    printF("RevolutionMapping::info: Coordinate axes are being re-ordered to be like the that of a sphere\n");
	    printF("RevolutionMapping::info: axis1=axial (phi), axis2=theta, axis3=radial\n");
	    setParameterAxes(axis,2,(axis+1)%2);
	  }
	  setTypeOfCoordinateSingularity( side,revAxis1,polarSingularity ); // phi has a "polar" singularity
	  setCoordinateEvaluationType( spherical,true );  // Mapping can be evaluated in spherical coordinates
	  if( getBoundaryCondition(side,revAxis1)<0 )
	    setBoundaryCondition(side,revAxis1,0);
	}
      }
      else
      {
	printF("RevolutionMapping::ERROR: invalid values for (side,axis)=(%i,%i)\n",side,axis);
        gi.stopReadingCommandFile();
	continue;
      }
      
    }
    else if( answer=="unset polar singularity" )
    {
      gi.inputString(line,"Enter the side,axis of the face to unset a polar singularity (2 integers)");
      int side=-1, axis=-1;
      sScanF(line,"%i %i",&side,&axis);
      if( side>=0 && side<=1 && axis>=0 && axis<domainDimension )
      {
	if( getTypeOfCoordinateSingularity( side,axis ) == polarSingularity )
	{
          setTypeOfCoordinateSingularity( side,axis,noCoordinateSingularity );
	}
	else
	{
          printF("RevolutionMapping::WARNING: This face (side,axis)=(%i,%i) does NOT currently have a "
                 "polar singularity\n",side,axis);
	}
      }
      else
      {
	printF("RevolutionMapping::ERROR: invalid values for (side,axis)=(%i,%i)\n",side,axis);
        gi.stopReadingCommandFile();
	continue;
      }
    }
    else if( answer=="show parameters" )
    {
      printf(" (startAngle,endAngle)=(%f,%f)\n",startAngle,endAngle);
      printf(" tangent to line to revolve about = (%f,%f,%f)\n",lineTangent(0),lineTangent(1),lineTangent(2));
      printf(" point on line to revolve about = (%f,%f,%f)\n",lineOrigin(0),lineOrigin(1),lineOrigin(2));
      printf(" revAxis1=%i revAxis2=%i revAxis3=%i\n",revAxis1,revAxis2,revAxis3);
      
      display();
      continue;
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
      continue;
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity" ||
	     answer=="check inverse")
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
      if( answer=="mappingName" )
        continue;
    }
    else if( answer=="check" )
    {
      checkMapping();
      
      Mapping::debug=31;
      realArray r(1,3),x(1,3),xr(1,3,3);
      r=0.;
      x=0;
      xr=0;
      for(;;)
      {
	gi.inputString(answer,"Evaluate at which point r? (hit return to continue)");
        if( answer!="" )
	{
	  sScanF(answer,"%e %e %e",&r(0,0),&r(0,1),&r(0,2));
          map(r,x,xr);
	  printf(" r=(%6.2e,%6.2e,%6.2e), x=(%6.2e,%6.2e,%6.2e), \n"
                 " xr=(%6.2e,%6.2e,%6.2e, %6.2e,%6.2e,%6.2e, %6.2e,%6.2e,%6.2e)\n",
             r(0,0),r(0,1),r(0,2), x(0,0),x(0,1),x(0,2), xr(0,0,0),xr(0,1,0),xr(0,2,0),
		 xr(0,0,1),xr(0,1,1),xr(0,2,1), xr(0,0,2),xr(0,1,2),xr(0,2,2));
	}
	else
	{
	  break;
	}
      }
    }
    else if( answer=="edit revolutionary" )
    {
      if( revolutionary!=NULL )
      {
	revolutionary->update(mapInfo);
      }
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }


    if( plotObject )
    {
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);   

      // plot the line of revolution
      LineMapping line(lineOrigin(0),lineOrigin(1),lineOrigin(2),
                       lineOrigin(0)+lineTangent(0),lineOrigin(1)+lineTangent(1),lineOrigin(2)+lineTangent(2));
      parameters.set(GI_USE_PLOT_BOUNDS,TRUE); 
      parameters.set(GraphicsParameters::curveLineWidth,3.);
      parameters.set(GI_MAPPING_COLOUR,"green");
      PlotIt::plot(gi,line,parameters);
      parameters.set(GI_MAPPING_COLOUR,"red"); // reset
      parameters.set(GI_USE_PLOT_BOUNDS,FALSE); 
      
    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}
