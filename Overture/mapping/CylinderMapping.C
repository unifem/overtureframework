#include "CylinderMapping.h"
#include "MappingInformation.h"
#include <float.h>

// temporary until Dan adds to A++
// *  realArray atan2( const realArray & y, const realArray & x );

//--------------------------------------------------------------------------
//  Here is a derived class to define a Cylindrical Surface or Volume in 3D
//--------------------------------------------------------------------------

CylinderMapping::
CylinderMapping(
		const real & startAngle_ /* = 0. */,
		const real & endAngle_ /* = 1. */,
		const real & startAxis_ /* = -1. */,
		const real & endAxis_ /* = +1. */,
                const real & innerRadius_ /* = 1. */, 
		const real & outerRadius_ /* = 1.5 */,
		const real & x0_ /* = 0. */, 
		const real & y0_ /* = 0. */, 
		const real & z0_ /* = 0. */, 
                const int & domainDimension_ /* = 3 */,
                const int & cylAxis1_ /* = axis1 */,
                const int & cylAxis2_ /* = axis2 */,
                const int & cylAxis3_ /* = axis3 */
                )
//===========================================================================
/// \brief  Create a 3D cylindrical volume or surface. 
/// \param Notes:
///  This mapping defines a cylinder in three-dimensions:
///  \[ \theta =  2\pi( \theta_0 + r_0( \theta_1-\theta_0) ) \]
///  \[ R(r_1) = (R_0 + r_2 (R_1-R_0))  \]
///  \[ {\mathbf x}(r_0,r_1,r_2) = ( R \cos(\theta ) + x_0 , R\sin(\theta) + y_0 , s_0 + r_1(s_1-s_0) + z_0 ) \]
/// 
///  The above cylinder has the z-axis as the axial direction. It is also possible to to have the
///  axial direction to point in any of the coordinate direction using the 
///  ({\tt cylAxis1}, {\tt cylAxis2}, {\tt cylAxis3}) variables (which should be a permutation of (0,1,2)):
///   Changing these variables will permute the definition of $(x_0,x_1,x_2)$: 
///  \[
///     (x_{\mathtt cylAxis1},x_{\mathtt cylAxis2},x_{\mathtt cylAxis3}) = ( R \cos(\theta ) 
///     + x_0 , R\sin(\theta) + y_0 , s_0 + r_2(s_1-s_0) + z_0 )
///  \]
///  NOTE that the parameter space coordinates are always $(\theta,\rm{axial},\rm{radial})$. 
/// 
/// \param startAngle (input) : starting angle ($\theta_0$) NOTE: angles are 1-periodic!
/// \param endAngle (input) :  ending angle ($\theta_1$) NOTE: angles are 1-periodic!.
/// \param startAxis (input) : axial coordinate of the start of the cylinder ($s_0$).
/// \param endAxis (input) :  axial coordinate of the end of the cylinder ($s_1$).
/// \param innerRadius (input) : inner radius ($R_0$).
/// \param outerRadius (input) : outer radius ($R_0$).
/// \param x0,y0,z0 (input) : center of the cylinder ($x_0$,$y_0$,$z_0$).
/// \param domainDimension (input) : 3 means the cylinder is a volume, 2 means the cylinder is a surface.
/// \param cylAxis1,cylAxis2,cylAxis3 (input) : change these to be a permutation of (axis1,axis2,axis3) to change
///    the orientation of the cylinder. NOTE: axis1==0, axis2==1, axis3==2.
//===========================================================================
  : Mapping(domainDimension_,3,parameterSpace,cartesianSpace)   
{ 
  CylinderMapping::className="CylinderMapping";

  setName( Mapping::mappingName,"Cylinder");
  setName(Mapping::domainAxis1Name,"theta");
  setName(Mapping::domainAxis2Name,"axial");
  setName(Mapping::domainAxis3Name,"radius");

  startAxis=startAxis_;  // these need to be set here first
  endAxis=endAxis_;
  setOrientation(cylAxis1_,cylAxis2_,cylAxis3_);

  setBasicInverseOption(canInvert);  // basicInverse is available
  inverseIsDistributed=false;

  setGridDimensions( axis1,21 );  // angle
  setGridDimensions( axis2,11 );  // axial
  if( domainDimension>2 )
    setGridDimensions( axis3,7  );  // radial

  setRadius(innerRadius_,outerRadius_);

  setOrigin(x0_,y0_,z0_);
  setAxis(startAxis_,endAxis_);
  setAngle(startAngle_,endAngle_);

  setBoundaryCondition( Start,axis2,3 );
  setBoundaryCondition( End  ,axis2,4 );
  if( domainDimension>2 )
  {
    setBoundaryCondition( Start,axis3,1 );
    setBoundaryCondition( End  ,axis3,2 );
  }  
  mappingHasChanged();
}


  // Copy constructor is deep by default
CylinderMapping::
CylinderMapping( const CylinderMapping & map, const CopyType copyType )
{
  CylinderMapping::className="CylinderMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "CylinderMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

CylinderMapping::
~CylinderMapping()
{ if( (debug/4) % 2 )
   cout << " CylinderMapping::Desctructor called" << endl;
}

CylinderMapping & CylinderMapping::
operator =( const CylinderMapping & X )
{
  if( CylinderMapping::className != X.getClassName() )
  {
    cout << "CylinderMapping::operator= ERROR trying to set a CylinderMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  innerRadius=X.innerRadius;
  outerRadius=X.outerRadius;
  x0=X.x0;
  y0=X.y0;
  z0=X.z0;
  startAxis=X.startAxis;
  endAxis=X.endAxis;
  startAngle=X.startAngle;
  endAngle=X.endAngle;
  cylAxis1=X.cylAxis1;
  cylAxis2=X.cylAxis2;
  cylAxis3=X.cylAxis3;
  

  return *this;
}

int CylinderMapping:: 
checkAxes()
{
      
  if( cylAxis1==cylAxis2 || cylAxis1==cylAxis3 || cylAxis2==cylAxis3 ||
      min(cylAxis1,cylAxis2,cylAxis3)<0 || max(cylAxis1,cylAxis2,cylAxis3)>2 )
  {
    cout << "CylinderMapping::ERROR (cylAxis1,cylAxis2,cylAxis3)=(%i,%i,%i) must be a permutation of (0,1,2)\n";
    cout << "CylinderMapping::setting back to (0,1,2)\n";
    cylAxis1=axis1;
    cylAxis2=axis2;
    cylAxis3=axis3;
    return 1;
  }
  else
    return 0;
}



int CylinderMapping:: 
setAngle(const real & startAngle_ /* =0. */, 
         const real & endAngle_ /* =1. */ )
//===========================================================================
/// \details 
///     Set the initial and final angles.
/// \param startAngle (input) : 
/// \param endAngle (input) : 
//===========================================================================
{
  startAngle=startAngle_;
  endAngle=endAngle_;
  if( fabs(endAngle-startAngle -1.) < REAL_EPSILON*10. )
  {
    setIsPeriodic(axis1, functionPeriodic );  
    setBoundaryCondition( Start,axis1,-1);
    setBoundaryCondition( End  ,axis1,-1);
  }
  else
  {
    setBoundaryCondition( Start,axis1,0 );
    setBoundaryCondition( End  ,axis1,0 );
    setIsPeriodic(axis1, notPeriodic );  
  }
  return 0;
}

int CylinderMapping:: 
setAxis(const real & startAxis_ /* =-1. */, 
	const real & endAxis_ /* =+1. */ )
//===========================================================================
/// \details 
///     Set the starting and ending axial positions.
/// \param startAxis (input) : axial coordinate of the start of the cylinder ($s_0$).
/// \param endAxis (input) :  axial coordinate of the end of the cylinder ($s_1$).
//===========================================================================
{
  startAxis=startAxis_;
  endAxis=endAxis_;
  updatePeriodVector();

  return 0;
}

int CylinderMapping:: 
updatePeriodVector()
//===========================================================================
// /Visibility: protected.
// /Description:
//    Update the periodVector in case the axial direction is `derivative periodic'
//===========================================================================
{
  setPeriodVector(0,axis2, 0. );
  setPeriodVector(1,axis2, 0. ); 
  setPeriodVector(2,axis2, 0. ); 

  setPeriodVector(axis3,axis2, endAxis-startAxis);  // in case we have derivativePeriodic

  return 0;
}

int CylinderMapping:: 
setOrientation( const int & cylAxis1_ /* =0 */,  
		const int & cylAxis2_ /* =1 */,  
		const int & cylAxis3_ /* =2 */ )
//===========================================================================
/// \details 
///     Set the orientation of the cylinder.
/// \param cylAxis1,cylAxis2,cylAxis3 (input) : change these to be a permutation of (axis1,axis2,axis3) to change
///    the orientation of the cylinder. NOTE: axis1==0, axis2==1, axis3==2.
//===========================================================================
{
  cylAxis1=cylAxis1_;
  cylAxis2=cylAxis2_;
  cylAxis3=cylAxis3_;
  checkAxes();
  updatePeriodVector();
  return 0;
}

int CylinderMapping:: 
setOrigin(const real & x0_ /* =0. */, 
	  const real & y0_ /* =0. */, 
	  const real & z0_ /* =0. */ )
//===========================================================================
/// \details 
///     Set the centre of the cylinder.
/// \param x0,y0,z0 (input) : center of the cylinder ($x_0$,$y_0$,$z_0$).
//===========================================================================
{
  x0=x0_;
  y0=y0_;
  z0=z0_;
  return 0;
}

int CylinderMapping:: 
setRadius(const real & innerRadius_ /* =1. */,
	  const real & outerRadius_ /* =1.5 */ )
//===========================================================================
/// \details 
///     Set the inner and outer radii.
/// \param innerRadius (input) : inner radius ($R_0$).
/// \param outerRadius (input) : outer radius ($R_0$).
//===========================================================================
{
  innerRadius=innerRadius_;
  outerRadius=outerRadius_;

  if( innerRadius==0. )
    setTypeOfCoordinateSingularity(Start,axis3,polarSingularity);
  if( outerRadius==0. )
    setTypeOfCoordinateSingularity(End,axis3,polarSingularity);

  return 0;
}




#define RADIUS(x) (rad*(x)+innerRadius)

void CylinderMapping::
map(const realArray & r, realArray & x, realArray & xr, MappingParameters & params)
{
  if( params.coordinateType != cartesian )
    cerr << "CylinderMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  const real scale=twoPi*(endAngle-startAngle);
  const real rad=outerRadius-innerRadius;
  const real length = endAxis-startAxis;
  const real xc[3] ={x0,y0,z0};  

  realArray angle(I);
  angle=scale*r(I,axis1)+startAngle*twoPi;

  if( computeMap )
  {
    if( domainDimension==2 )
    {
      x(I,cylAxis1)=innerRadius*cos(angle)+xc[cylAxis1]; 
      x(I,cylAxis2)=innerRadius*sin(angle)+xc[cylAxis2];
      x(I,cylAxis3)=length*r(I,axis2)+startAxis+xc[cylAxis3];
    }
    else
    {
      x(I,cylAxis1)=RADIUS(r(I,axis3))*cos(angle)+xc[cylAxis1]; 
      x(I,cylAxis2)=RADIUS(r(I,axis3))*sin(angle)+xc[cylAxis2];
      x(I,cylAxis3)=length*r(I,axis2)+startAxis+xc[cylAxis3];
    }
  }
  if( computeMapDerivative )
  {
    if( domainDimension==2 )
    {
      xr(I,cylAxis1,axis1)=-innerRadius*scale*sin(angle);
      xr(I,cylAxis2,axis1)= innerRadius*scale*cos(angle);
      xr(I,cylAxis3,axis1)=0;

      xr(I,cylAxis1,axis2)=0.;
      xr(I,cylAxis2,axis2)=0.;
      xr(I,cylAxis3,axis2)=length;
    }
    else
    {
      // **add** if( radius.getBase(0)>base || radius.getBound(0)<bound )
      realArray radius(I);
      radius=RADIUS(r(I,axis3));

      xr(I,cylAxis1,axis1)=-radius*scale*sin(angle);
      xr(I,cylAxis2,axis1)= radius*scale*cos(angle);
      xr(I,cylAxis3,axis1)=0;

      xr(I,cylAxis1,axis2)=0.;
      xr(I,cylAxis2,axis2)=0.;
      xr(I,cylAxis3,axis2)=length;

      xr(I,cylAxis1,axis3)= rad*cos(angle);
      xr(I,cylAxis2,axis3)= rad*sin(angle);
      xr(I,cylAxis3,axis3)=0.;
    }
  }
}

void CylinderMapping::
mapS(const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params)
{
  if( params.coordinateType != cartesian )
    cerr << "CylinderMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  const real scale=twoPi*(endAngle-startAngle);
  const real rad=outerRadius-innerRadius;
  const real length = endAxis-startAxis;
  const real xc[3] ={x0,y0,z0};  

  RealArray angle(I);
  angle=scale*r(I,axis1)+startAngle*twoPi;

  if( computeMap )
  {
    if( domainDimension==2 )
    {
      x(I,cylAxis1)=innerRadius*cos(angle)+xc[cylAxis1]; 
      x(I,cylAxis2)=innerRadius*sin(angle)+xc[cylAxis2];
      x(I,cylAxis3)=length*r(I,axis2)+startAxis+xc[cylAxis3];
    }
    else
    {
      x(I,cylAxis1)=RADIUS(r(I,axis3))*cos(angle)+xc[cylAxis1]; 
      x(I,cylAxis2)=RADIUS(r(I,axis3))*sin(angle)+xc[cylAxis2];
      x(I,cylAxis3)=length*r(I,axis2)+startAxis+xc[cylAxis3];
    }
  }
  if( computeMapDerivative )
  {
    if( domainDimension==2 )
    {
      xr(I,cylAxis1,axis1)=-innerRadius*scale*sin(angle);
      xr(I,cylAxis2,axis1)= innerRadius*scale*cos(angle);
      xr(I,cylAxis3,axis1)=0;

      xr(I,cylAxis1,axis2)=0.;
      xr(I,cylAxis2,axis2)=0.;
      xr(I,cylAxis3,axis2)=length;
    }
    else
    {
      // **add** if( radius.getBase(0)>base || radius.getBound(0)<bound )
      RealArray radius(I);
      radius=RADIUS(r(I,axis3));

      xr(I,cylAxis1,axis1)=-radius*scale*sin(angle);
      xr(I,cylAxis2,axis1)= radius*scale*cos(angle);
      xr(I,cylAxis3,axis1)=0;

      xr(I,cylAxis1,axis2)=0.;
      xr(I,cylAxis2,axis2)=0.;
      xr(I,cylAxis3,axis2)=length;

      xr(I,cylAxis1,axis3)= rad*cos(angle);
      xr(I,cylAxis2,axis3)= rad*sin(angle);
      xr(I,cylAxis3,axis3)=0.;
    }
  }
}


#undef RADIUS

void CylinderMapping::
basicInverse(const realArray & x, realArray & r, realArray & rx, MappingParameters & params)
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  const real inverseScale=1./(twoPi*(endAngle-startAngle));
  const real rad=outerRadius-innerRadius;
  const real inverseRad=1./rad;
  const real inverseLength = 1./(endAxis-startAxis);
  const real xc[3] ={x0,y0,z0}; 

  if( computeMap )
  {
    // *wdh* r(I,axis1)=(atan2(evaluate(xc[cylAxis2]-x(I,cylAxis2)),evaluate(xc[cylAxis1]-x(I,cylAxis1)))+(Pi-twoPi*startAngle))*inverseScale;
    real theta0=twoPi*startAngle; //  theta1=twoPi*endAngle;
    if( getIsPeriodic(axis1) )
    {
      // ***NOTE evaluate atan2(-y/-x) gives theta +/- pi
      r(I,axis1)=atan2(evaluate(xc[cylAxis2]-x(I,cylAxis2)),evaluate(xc[cylAxis1]-x(I,cylAxis1))); // **NOTE** (-y,-x) : result in [-pi,pi]
      r(I,axis1)=( r(I,axis1)+(Pi-theta0) )*inverseScale;
      r(I,axis1)=fmod(r(I,axis1)+1.,1.);  // map back to [0,1]
    }
    else
    {
      // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
      // delta is the gap in the angle between the start and end of the annulus
      r(I,axis1)=atan2(evaluate(x(I,cylAxis2)-xc[cylAxis2]),evaluate(x(I,cylAxis1)-xc[cylAxis1]));  // **NOTE** +theta : result in [-pi,pi]
      real delta = (1.-(endAngle-startAngle))*Pi;
      where ( r(I,axis1) < theta0 - delta )
      {
	r(I,axis1)+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
      }
      r(I,axis1)=(r(I,axis1)-theta0)*inverseScale;
    }



    r(I,axis2)=(x(I,cylAxis3)-xc[cylAxis3]-startAxis)*inverseLength;
    if( domainDimension==3 )
      r(I,axis3)=(sqrt(SQR(x(I,cylAxis1)-xc[cylAxis1])+SQR(x(I,cylAxis2)-xc[cylAxis2]))-innerRadius)*inverseRad;
//      r(I,axis3)=(pow(pow(x(I,cylAxis1)-xc[cylAxis1],2)+pow(x(I,cylAxis2)-xc[cylAxis2],2),.5)-innerRadius)*inverseRad;

    periodicShift(r,I);   // shift r in any periodic directions
  }
  if( computeMapDerivative )
  {
    // ***add** if( radius.getBase(0)>base || radius.getBound(0)<bound )
    realArray radius(I);

    if( domainDimension==2 )
    {
      for( int i=base; i<=bound; i++ )
      {
      }
    }
    else
    {
      // radius=pow(pow(x(I,cylAxis1)-xc[cylAxis1],2)+pow(x(I,cylAxis2)-xc[cylAxis2],2),.5);
      radius=sqrt( SQR(x(I,cylAxis1)-xc[cylAxis1]) + SQR(x(I,cylAxis2)-xc[cylAxis2]) );
      rx(I,axis3,cylAxis1)= (x(I,cylAxis1)-xc[cylAxis1])/(rad*radius);
      rx(I,axis3,cylAxis2)= (x(I,cylAxis2)-xc[cylAxis2])/(rad*radius);
      rx(I,axis3,cylAxis3)=0.;
      radius=inverseScale/(SQR(radius));    // ** change defn of radius! **
      rx(I,axis1,cylAxis1)=-(x(I,cylAxis2)-xc[cylAxis2])*radius;
      rx(I,axis1,cylAxis2)= (x(I,cylAxis1)-xc[cylAxis1])*radius;
      rx(I,axis1,cylAxis3)=0.;

      rx(I,axis2,cylAxis1)=0.;
      rx(I,axis2,cylAxis2)=0.;
      rx(I,axis2,cylAxis3)=inverseLength;
    }
  }
}

void CylinderMapping::
basicInverseS(const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params)
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  const real inverseScale=1./(twoPi*(endAngle-startAngle));
  const real rad=outerRadius-innerRadius;
  const real inverseRad=1./rad;
  const real inverseLength = 1./(endAxis-startAxis);
  const real xc[3] ={x0,y0,z0}; 

  if( computeMap )
  {
    // *wdh* r(I,axis1)=(atan2(evaluate(xc[cylAxis2]-x(I,cylAxis2)),evaluate(xc[cylAxis1]-x(I,cylAxis1)))+(Pi-twoPi*startAngle))*inverseScale;
    real theta0=twoPi*startAngle; //  theta1=twoPi*endAngle;
    if( getIsPeriodic(axis1) )
    {
      // ***NOTE evaluate atan2(-y/-x) gives theta +/- pi
      r(I,axis1)=atan2(evaluate(xc[cylAxis2]-x(I,cylAxis2)),evaluate(xc[cylAxis1]-x(I,cylAxis1))); // **NOTE** (-y,-x) : result in [-pi,pi]
      r(I,axis1)=( r(I,axis1)+(Pi-theta0) )*inverseScale;
      r(I,axis1)=fmod(r(I,axis1)+1.,1.);  // map back to [0,1]
    }
    else
    {
      // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
      // delta is the gap in the angle between the start and end of the annulus
      r(I,axis1)=atan2(evaluate(x(I,cylAxis2)-xc[cylAxis2]),evaluate(x(I,cylAxis1)-xc[cylAxis1]));  // **NOTE** +theta : result in [-pi,pi]
      real delta = (1.-(endAngle-startAngle))*Pi;
      where ( r(I,axis1) < theta0 - delta )
      {
	r(I,axis1)+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
      }
      r(I,axis1)=(r(I,axis1)-theta0)*inverseScale;
    }



    r(I,axis2)=(x(I,cylAxis3)-xc[cylAxis3]-startAxis)*inverseLength;
    if( domainDimension==3 )
      r(I,axis3)=(sqrt(SQR(x(I,cylAxis1)-xc[cylAxis1])+SQR(x(I,cylAxis2)-xc[cylAxis2]))-innerRadius)*inverseRad;
    // r(I,axis3)=(pow(pow(x(I,cylAxis1)-xc[cylAxis1],2)+pow(x(I,cylAxis2)-xc[cylAxis2],2),.5)-innerRadius)*inverseRad;

    periodicShift(r,I);   // shift r in any periodic directions
  }
  if( computeMapDerivative )
  {
    // ***add** if( radius.getBase(0)>base || radius.getBound(0)<bound )
    RealArray radius(I);

    if( domainDimension==2 )
    {
      for( int i=base; i<=bound; i++ )
      {
      }
    }
    else
    {
      radius=sqrt(SQR(x(I,cylAxis1)-xc[cylAxis1])+SQR(x(I,cylAxis2)-xc[cylAxis2]));
      // radius=pow(pow(x(I,cylAxis1)-xc[cylAxis1],2)+pow(x(I,cylAxis2)-xc[cylAxis2],2),.5);
      rx(I,axis3,cylAxis1)= (x(I,cylAxis1)-xc[cylAxis1])/(rad*radius);
      rx(I,axis3,cylAxis2)= (x(I,cylAxis2)-xc[cylAxis2])/(rad*radius);
      rx(I,axis3,cylAxis3)=0.;
      radius=inverseScale/(SQR(radius));    // ** change defn of radius! **
      rx(I,axis1,cylAxis1)=-(x(I,cylAxis2)-xc[cylAxis2])*radius;
      rx(I,axis1,cylAxis2)= (x(I,cylAxis1)-xc[cylAxis1])*radius;
      rx(I,axis1,cylAxis3)=0.;

      rx(I,axis2,cylAxis1)=0.;
      rx(I,axis2,cylAxis2)=0.;
      rx(I,axis2,cylAxis3)=inverseLength;
    }
  }
}



// get a mapping from the database
int CylinderMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( (debug/4) % 2 )
    cout << "Entering CylinderMapping::get" << endl;

  subDir.get( CylinderMapping::className,"className" ); 
  if( CylinderMapping::className != "CylinderMapping" )
  {
    cout << "CylinderMapping::get ERROR in className!" << endl;
    cout << "className from the database = " << CylinderMapping::className << endl;
  }

  subDir.get( innerRadius,"innerRadius" );
  subDir.get( outerRadius,"outerRadius" );
  subDir.get( x0,"x0" );
  subDir.get( y0,"y0" );
  subDir.get( z0,"z0" );
  subDir.get( startAxis,"startAxis" );
  subDir.get( endAxis,"endAxis" );
  subDir.get( startAngle,"startAngle" );
  subDir.get( endAngle,"endAngle" );
  subDir.get( cylAxis1,"cylAxis1" );
  subDir.get( cylAxis2,"cylAxis2" );
  subDir.get( cylAxis3,"cylAxis3" );

  Mapping::get( subDir, "Mapping" );
  mappingHasChanged();
  delete &subDir;
  return 0;
}
int CylinderMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( CylinderMapping::className,"className" );

  subDir.put( innerRadius,"innerRadius" );
  subDir.put( outerRadius,"outerRadius" );
  subDir.put( x0,"x0" );
  subDir.put( y0,"y0" );
  subDir.put( z0,"z0" );
  subDir.put( startAxis,"startAxis" );
  subDir.put( endAxis,"endAxis" );
  subDir.put( startAngle,"startAngle" );
  subDir.put( endAngle,"endAngle" );
  subDir.put( cylAxis1,"cylAxis1" );
  subDir.put( cylAxis2,"cylAxis2" );
  subDir.put( cylAxis3,"cylAxis3" );

  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping* CylinderMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==CylinderMapping::className )
    retval = new CylinderMapping();
  return retval;
}

aString CylinderMapping::
getClassName() const
{
  return CylinderMapping::className;
}

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int CylinderMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!CylinderMapping",
      "centre for cylinder",
      "bounds on theta",
      "bounds on the axial variable",
      "bounds on the radial variable",
      "surface or volume (toggle)",
      "orientation",
      " ",
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
      "centre for cylinder axis : Specify the centre of the cylinder",
      "bounds on theta    : Set the bounds on the angular variable starts, in [0,1]",
      "bounds on the axial variable : specify bounds on the length of the cylinder",
      "bounds on the radial variable: Specify the inner and outer radius",
      "surface or volume (toggle) :",
      "orientation : change the orientation of the cylinder",
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

  aString answer,line; 

  bool plotObject=TRUE;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("Cylinder>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="centre for cylinder" || answer=="center for cylinder" ) 
    {
      gi.inputString(line,sPrintF(buff,"Enter (x0,y0,z0) for centre (default=(%e,%e,%e)): ",x0,y0,z0));
      if( line!="" ) sScanF(line,"%e %e %e",&x0,&y0,&z0);
      setOrigin(x0,y0,z0);
      mappingHasChanged();
    }
    else if( answer=="bounds on the axial variable" )
    {
      gi.inputString(line,sPrintF(buff,"Enter start and end for the axis (default=%e,%e): ",
        startAxis,endAxis));
      if( line!="" ) sScanF( line,"%e %e",&startAxis,&endAxis);
      setAxis(startAxis,endAxis);
      mappingHasChanged();
    }
    else if( answer=="bounds on the radial variable" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the inner and outer radii (default=%e,%e): ",
          innerRadius,outerRadius));
      if( line!="" ) sScanF( line,"%e %e",&innerRadius,&outerRadius);
      setRadius(innerRadius,outerRadius);
      mappingHasChanged();
    }
    else if( answer=="bounds on theta" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the bounds on `theta' in [0,1] (default=%e,%e): ",
          startAngle,endAngle));
      if( line!="" ) sScanF( line,"%e %e",&startAngle,&endAngle);
      // If the annulus is no longer closed, reset BC's and periodicity
      setAngle(startAngle,endAngle);
/* ---
      if( fabs(endAngle-startAngle-1.) > REAL_EPSILON*10. &&   // annulus is not closed
	 getIsPeriodic(axis1)                       )  
      {
        gi.outputString("Cylinder is nolonger periodic along axis1, changing periodicity and boundary conditions");
	setIsPeriodic(axis1,notPeriodic );  
	if( getBoundaryCondition(Start,axis1)<0 )
	{
	  setBoundaryCondition(Start,axis1,0);
	  setBoundaryCondition(  End,axis1,0);
	}
      }
--- */
      mappingHasChanged();
    }
    else if(answer=="surface or volume (toggle)")
    {
      if( domainDimension==3 )
        setDomainDimension(2);
      else
        setDomainDimension(3);
      mappingHasChanged();
    }
    else if(answer=="orientation" )
    {
      printf(" orientation: 0,1,2 : axial direction is 2, cylinder is along the z-axis\n"
             "              1,2,0 : axial direction is 0, cylinder is along the x-axis.\n"
             "              2,0,1 : axial direction is 1, cylinder is along the y-axis.\n");
      gi.inputString(line,sPrintF(buff,"Input the orientation a,b,c (a permutation of 0,1,2) (default=%i,%i,%i): ",
          cylAxis1,cylAxis2,cylAxis3));
      if( line!="" ) sScanF( line,"%i %i %i",&cylAxis1,&cylAxis2,&cylAxis3);
      setOrientation(cylAxis1,cylAxis2,cylAxis3);
      mappingHasChanged();
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
    else if( answer=="show parameters" )
    {
      printf(" (innerRadius,outerRadius=(%e,%e)\n centre: (x0,y0,z0)=(%e,%e,%e)\n",
         innerRadius,outerRadius,x0,y0,z0);
      printf(" (startAngle,endAngle)=(%e,%e)\n",startAngle,endAngle);
      Mapping::display(); 
      
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
      PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***
    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
}

