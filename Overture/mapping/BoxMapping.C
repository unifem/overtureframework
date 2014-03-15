#include "BoxMapping.h"
#include "MappingInformation.h"
#include "GenericDataBase.h"


BoxMapping::
BoxMapping( 
        const real xMin, const real xMax, 
        const real yMin, const real yMax,
        const real zMin, const real zMax ) : Mapping(3,3)
/// -------------------------------------------------------------
/// \details 
///     Build a rectangular box in 3D. The box can also be rotated
///   around one the coordinate directions.
/// \param xMin,xMax : minimum and maximum values for x(xAxis)
/// \param yMin,yMax : minimum and maximum values for y(yAxis)
/// \param zMin,zMax : minimum and maximum values for z(zAxis)
//-------------------------------------------------------------
{ 
  BoxMapping::className="BoxMapping";         
  setName( Mapping::mappingName,"box");

  rotationAxis=2;
  rotationAngle=0.; 
  centerOfRotation[0]=0.;
  centerOfRotation[1]=0.;
  centerOfRotation[2]=0.;

  setVertices(xMin,xMax,yMin,yMax,zMin,zMax);
  for( int axis=axis1; axis<=axis3; axis++ )
    for( int side=Start; side<=End; side++ )
      setBoundaryCondition( side,axis,1 ); 
  setDomainSpace( parameterSpace );   
  setRangeSpace( cartesianSpace );  
  setMappingCoordinateSystem( rectangular );  // for optimizing derivatives
  if( fabs((xMax-xMin)*(yMax-yMin)*(zMax-zMin)) > 0. )
  {
    setInvertible( true );
    setBasicInverseOption(canInvert);    
    inverseIsDistributed=false;
  }
  else
    setInvertible( false );

  

  mappingHasChanged();
}


// Copy constructor is deep by default
BoxMapping::
BoxMapping( const BoxMapping & map, const CopyType copyType )
{
  BoxMapping::className="BoxMapping";         
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "BoxMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

BoxMapping::
~BoxMapping()
{ if( (Mapping::debug/4) % 2 )
     cout << " BoxMapping::Desctructor called" << endl;
}

BoxMapping & BoxMapping::
operator =( const BoxMapping & x )
{
  this->Mapping::operator=(x);            // call = for derivee class

  for( int axis=0; axis<3; axis++ )
  {
    xa[axis]=x.xa[axis];
    xb[axis]=x.xb[axis];
    centerOfRotation[axis]=x.centerOfRotation[axis];
  }
  rotationAxis=x.rotationAxis;
  rotationAngle=x.rotationAngle;
  
  return *this;
}

RealArray BoxMapping::
getBoundingBox( const int & side /* =-1 */, const int & axis /* =-1 */ ) const
// =====================================================================================
// 
// Specialized version. Supplying this functions means we can avoid building the grid in
// order to determine the bounding box for intersect etc.
// 
// /Description:
//   Return the bounding box for the Mapping (if side<0 and axis<0) or the bounding
//   box for a particular side.
//   /side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
//     and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{
  RealArray bb(2,3);
  for( int dir=0; dir<3; dir++ )
  {
    bb(0,dir)=xa[dir];
    bb(1,dir)=xb[dir];
  }  
  if( side<0 && axis<0 )
  {
    return bb;
  }
  
  if( !validSide( side ) || !validAxis( axis ) )
  {
    cout << " BoxMapping::getBoundingBox: Invalid arguments " << endl;
    Overture::abort("error");
  }

  bb(0,axis)=bb(side,axis);
  bb(1,axis)=bb(side,axis);
  return bb;
  
//  return Mapping::getBoundingBox(side,axis);
}

int BoxMapping::
getBoundingBox( const IntegerArray & indexRange, const IntegerArray & gridIndexRange_,
                RealArray & xBounds, bool local /* =false */ ) const
// =====================================================================================
// 
// Specialized version. Supplying this functions means we can avoid building the grid in
// order to determine the bounding box for intersect etc.
// 
// /Description:
//   Return the bounding box, xBounds, for the set of grid points spanned by 
//   indexRange. 
//
// /indexRange(0:1,0:2) (input) : range of indicies, i\_m=indexRange(0,m),...,indexRange(1,m)
// /gridIndexRange\_(0:1,0:2) (input) : Normally these should match the gridIndexRange of the Mapping.
//    This argument is used to double check that this is true.
// /xBounds(0:1,0:2) : bounds
// /local (input) : if local=true then only compute the min and max over points on this processor, otherwise
//                  compute the min and max over all points on all processors
//
// /Return values: 0=success, 1=indexRange values are invalid.
// =====================================================================================
{
  for( int dir=0; dir<3; dir++ )
  {
    real dx = (xb[dir]-xa[dir])/max(1,gridIndexRange_(1,dir)-gridIndexRange_(0,dir));
    xBounds(0,dir)=xa[dir]+ (indexRange(0,dir)-gridIndexRange_(0,dir))*dx;
    xBounds(1,dir)=xb[dir]+ (indexRange(1,dir)-gridIndexRange_(1,dir))*dx;
  }  

  return 0;
}


int BoxMapping::
getBoundingBox( const RealArray & rBounds, RealArray & xBounds ) const
// =====================================================================================
// 
// Specialized version. Supplying this functions means we can avoid building the grid in
// order to determine the bounding box for intersect etc.
// 
// /Description:
//   Return the bounding box, xBounds, for the range space that corresponds to the
//   bounding box, rBounds, in the domain space. 
// =====================================================================================
{
  return Mapping::getBoundingBox(rBounds,xBounds);
}


int BoxMapping::
rotate(const real angle, 
       const int axisOfRotation /* = 2 */,
       const real x0 /* = 0. */,
       const real y0 /* = 0. */,
       const real z0 /* = 0. */ )
//===========================================================================
/// \details 
///     Rotate the box around a coordinate direction.
/// \param angle (input) : angle in degrees.
/// \param xisOfRotation (input) : 0,1 or 2.
/// \param x0,y0,z0 (input) : rotate about this point.
//===========================================================================
{
  rotationAngle=angle;
  rotationAxis=axisOfRotation;
  if( rotationAxis<0 || rotationAxis>2 )
  {
    cout << "BoxMapping::Invalid rotation axis = " << rotationAxis << endl;
    rotationAxis=2;
  }
  centerOfRotation[0]=x0;
  centerOfRotation[1]=y0;
  centerOfRotation[2]=z0;

  setVertices(xa[0],xb[0],xa[1],xb[1],xa[2],xb[2]); // this will set the period vector.
  
  return 0;
}


int  BoxMapping::
getVertices(real & xMin, real & xMax, real & yMin, real & yMax, real & zMin, real & zMax ) const
//===========================================================================
/// \brief  Return the bounds on the box.
/// \param xMin, xMax, yMin, yMax, zMin, zMax (output) : bounds on the box.
//===========================================================================
{
  xMin=xa[0]; xMax=xb[0]; 
  yMin=xa[1]; yMax=xb[1]; 
  zMin=xa[2]; zMax=xb[2]; 

  return 0;
}


int BoxMapping::
setVertices(const real & xMin /* =0. */,
	    const real & xMax /* =1. */, 
	    const real & yMin /* =0. */, 
	    const real & yMax /* =1. */,
	    const real & zMin /* =0. */,
	    const real & zMax /* =1. */)
//===========================================================================
/// \brief  Set the bounds on the box.
/// \param xMin, xMax, yMin, yMax, zMin, zMax (input) : bounds on the box.
//===========================================================================
{
  xa[0]=xMin; xb[0]=xMax; 
  xa[1]=yMin; xb[1]=yMax; 
  xa[2]=zMin; xb[2]=zMax; 

  int axis,dir;
  for( axis=0; axis<3; axis++ )
    for( dir=0; dir<3; dir++ )
      setPeriodVector(axis,dir,0.);
  if( rotationAngle==0. )
  {
    for( axis=0; axis<3; axis++ )
      setPeriodVector(axis,axis,xb[axis]-xa[axis]);  // in case we have derivativePeriodic
  }
  else
  {

    setPeriodVector(rotationAxis,rotationAxis,xb[rotationAxis]-xa[rotationAxis]); 
    const int dir1 = (rotationAxis+1) % 3;
    const int dir2 = (rotationAxis+2) % 3;
    const real theta=rotationAngle*Pi/180.;
    const real cosTheta=cos(theta);
    const real sinTheta=sin(theta);

    setPeriodVector(dir1,dir1, (xb[dir1]-xa[dir1])*cosTheta); 
    setPeriodVector(dir2,dir1,-(xb[dir2]-xa[dir2])*sinTheta); 

    setPeriodVector(dir1,dir2, (xb[dir1]-xa[dir1])*sinTheta); 
    setPeriodVector(dir2,dir2, (xb[dir2]-xa[dir2])*cosTheta); 

  }
  mappingHasChanged();
  return 0;
}



// ---get a mapping from the database---
int BoxMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( (Mapping::debug/4) % 2 )
    cout << "Entering BoxMapping::get" << endl;

  subDir.get( BoxMapping::className,"className" ); 
  if( BoxMapping::className != "BoxMapping" )
  {
    cout << "BoxMapping::get ERROR in className!" << endl;
  }
  subDir.get( xa,"xa",3 );
  subDir.get( xb,"xb",3 );
  subDir.get( rotationAxis,"rotationAxis" );
  subDir.get( rotationAngle,"rotationAngle" );
  subDir.get( centerOfRotation,"centerOfRotation",3 );
  Mapping::get( subDir, "Mapping" );
  mappingHasChanged();
  delete & subDir;
  return 0;
}

int BoxMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( BoxMapping::className,"className" );
  subDir.put( xa,"xa",3 );
  subDir.put( xb,"xb",3 );
  subDir.put( rotationAxis,"rotationAxis" );
  subDir.put( rotationAngle,"rotationAngle" );
  subDir.put( centerOfRotation,"centerOfRotation",3 );

  Mapping::put( subDir, "Mapping" );
  delete & subDir;
  return 0;
}

Mapping* BoxMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=NULL;
  if( mappingClassName==BoxMapping::className )
  {
    retval = new BoxMapping();
    assert( retval != 0 );
  }
  return retval;
}

void BoxMapping::
map( const realArray & r, realArray & x, realArray & xr,
                      MappingParameters & params )
{ 
  if( params.coordinateType != cartesian )
    cerr << "BoxMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    x(I,axis1)=xa[0]+(xb[0]-xa[0])*r(I,axis1); 
    x(I,axis2)=xa[1]+(xb[1]-xa[1])*r(I,axis2); 
    x(I,axis3)=xa[2]+(xb[2]-xa[2])*r(I,axis3);
    if( rotationAngle!=0. )
    {
      const int dir1 = (rotationAxis+1) % 3;
      const int dir2 = (rotationAxis+2) % 3;
      const real theta=rotationAngle*Pi/180.;
      const real cosTheta=cos(theta);
      const real sinTheta=sin(theta);
      
      x(I,dir1)-=centerOfRotation[dir1];
      x(I,dir2)-=centerOfRotation[dir2];
      realArray temp = x(I,dir1)*cosTheta-x(I,dir2)*sinTheta +centerOfRotation[dir1];
      x(I,dir2)=       x(I,dir1)*sinTheta+x(I,dir2)*cosTheta +centerOfRotation[dir2];
      x(I,dir1)=temp;
    }
  }
  if( computeMapDerivative )
  {
    xr=0; 
    if( rotationAngle==0. )
    {
      xr(I,axis1,axis1)=xb[0]-xa[0];
      xr(I,axis2,axis2)=xb[1]-xa[1];
      xr(I,axis3,axis3)=xb[2]-xa[2];
    }
    else
    {
      const int dir1 = (rotationAxis+1) % 3;
      const int dir2 = (rotationAxis+2) % 3;
      const real theta=rotationAngle*Pi/180.;
      const real cosTheta=cos(theta);
      const real sinTheta=sin(theta);
      
      xr(I,rotationAxis,rotationAxis)=xb[rotationAxis]-xa[rotationAxis];
      xr(I,dir1,dir1)= (xb[dir1]-xa[dir1])*cosTheta;
      xr(I,dir1,dir2)=-(xb[dir2]-xa[dir2])*sinTheta;
      xr(I,dir2,dir1)= (xb[dir1]-xa[dir1])*sinTheta;
      xr(I,dir2,dir2)= (xb[dir2]-xa[dir2])*cosTheta;
    }
  }
}

void BoxMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr,
                      MappingParameters & params )
{ 
  if( params.coordinateType != cartesian )
    cerr << "BoxMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    x(I,axis1)=xa[0]+(xb[0]-xa[0])*r(I,axis1); 
    x(I,axis2)=xa[1]+(xb[1]-xa[1])*r(I,axis2); 
    x(I,axis3)=xa[2]+(xb[2]-xa[2])*r(I,axis3);
    if( rotationAngle!=0. )
    {
      const int dir1 = (rotationAxis+1) % 3;
      const int dir2 = (rotationAxis+2) % 3;
      const real theta=rotationAngle*Pi/180.;
      const real cosTheta=cos(theta);
      const real sinTheta=sin(theta);
      
      x(I,dir1)-=centerOfRotation[dir1];
      x(I,dir2)-=centerOfRotation[dir2];
      RealArray temp;
      temp = x(I,dir1)*cosTheta-x(I,dir2)*sinTheta +centerOfRotation[dir1];
      x(I,dir2)=       x(I,dir1)*sinTheta+x(I,dir2)*cosTheta +centerOfRotation[dir2];
      x(I,dir1)=temp;
    }
  }
  if( computeMapDerivative )
  {
    xr=0; 
    if( rotationAngle==0. )
    {
      xr(I,axis1,axis1)=xb[0]-xa[0];
      xr(I,axis2,axis2)=xb[1]-xa[1];
      xr(I,axis3,axis3)=xb[2]-xa[2];
    }
    else
    {
      const int dir1 = (rotationAxis+1) % 3;
      const int dir2 = (rotationAxis+2) % 3;
      const real theta=rotationAngle*Pi/180.;
      const real cosTheta=cos(theta);
      const real sinTheta=sin(theta);
      
      xr(I,rotationAxis,rotationAxis)=xb[rotationAxis]-xa[rotationAxis];
      xr(I,dir1,dir1)= (xb[dir1]-xa[dir1])*cosTheta;
      xr(I,dir1,dir2)=-(xb[dir2]-xa[dir2])*sinTheta;
      xr(I,dir2,dir1)= (xb[dir1]-xa[dir1])*sinTheta;
      xr(I,dir2,dir2)= (xb[dir2]-xa[dir2])*cosTheta;
    }
  }
}

void BoxMapping::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    if( rotationAngle==0. )
    {
      r(I,axis1)=(x(I,axis1)-xa[0])/(xb[0]-xa[0]);
      r(I,axis2)=(x(I,axis2)-xa[1])/(xb[1]-xa[1]); 
      r(I,axis3)=(x(I,axis3)-xa[2])/(xb[2]-xa[2]);
    }
    else
    {
      const int dir1 = (rotationAxis+1) % 3;
      const int dir2 = (rotationAxis+2) % 3;
      const real theta=rotationAngle*Pi/180.;
      const real cosTheta=cos(theta);
      const real sinTheta=sin(theta);

      realArray t1 = x(I,dir1)-centerOfRotation[dir1];
      realArray t2 = x(I,dir2)-centerOfRotation[dir2];
      
      r(I,dir1)=( t1*cosTheta+t2*sinTheta+(-xa[dir1]+centerOfRotation[dir1]))/(xb[dir1]-xa[dir1]);
      r(I,dir2)=(-t1*sinTheta+t2*cosTheta+(-xa[dir2]+centerOfRotation[dir2]))/(xb[dir2]-xa[dir2]);

      r(I,rotationAxis)=(x(I,rotationAxis)-xa[rotationAxis])/(xb[rotationAxis]-xa[rotationAxis]);

    }
    
    periodicShift(r,I);   // shift r in any periodic directions
  }
  if( computeMapDerivative )
  {
    rx=0; 
    if( rotationAngle==0. )
    {
      rx(I,xAxis,xAxis)=1./(xb[0]-xa[0]); 
      rx(I,yAxis,yAxis)=1./(xb[1]-xa[1]); 
      rx(I,zAxis,zAxis)=1./(xb[2]-xa[2]); 
    }
    else
    {
      const int dir1 = (rotationAxis+1) % 3;
      const int dir2 = (rotationAxis+2) % 3;
      const real theta=rotationAngle*Pi/180.;
      const real cosTheta=cos(theta);
      const real sinTheta=sin(theta);

      rx(I,rotationAxis,rotationAxis)=1./(xb[rotationAxis]-xa[rotationAxis]);
      rx(I,dir1,dir1)= cosTheta/(xb[dir1]-xa[dir1]);
      rx(I,dir1,dir2)= sinTheta/(xb[dir1]-xa[dir1]);
      rx(I,dir2,dir1)=-sinTheta/(xb[dir2]-xa[dir2]);
      rx(I,dir2,dir2)= cosTheta/(xb[dir2]-xa[dir2]);
    }
    
  }
}


void BoxMapping::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    if( rotationAngle==0. )
    {
      r(I,axis1)=(x(I,axis1)-xa[0])/(xb[0]-xa[0]);
      r(I,axis2)=(x(I,axis2)-xa[1])/(xb[1]-xa[1]); 
      r(I,axis3)=(x(I,axis3)-xa[2])/(xb[2]-xa[2]);
    }
    else
    {
      const int dir1 = (rotationAxis+1) % 3;
      const int dir2 = (rotationAxis+2) % 3;
      const real theta=rotationAngle*Pi/180.;
      const real cosTheta=cos(theta);
      const real sinTheta=sin(theta);

      RealArray t1 = x(I,dir1)-centerOfRotation[dir1];
      RealArray t2 = x(I,dir2)-centerOfRotation[dir2];
      
      r(I,dir1)=( t1*cosTheta+t2*sinTheta+(-xa[dir1]+centerOfRotation[dir1]))/(xb[dir1]-xa[dir1]);
      r(I,dir2)=(-t1*sinTheta+t2*cosTheta+(-xa[dir2]+centerOfRotation[dir2]))/(xb[dir2]-xa[dir2]);

      r(I,rotationAxis)=(x(I,rotationAxis)-xa[rotationAxis])/(xb[rotationAxis]-xa[rotationAxis]);

    }
    
    periodicShift(r,I);   // shift r in any periodic directions
  }
  if( computeMapDerivative )
  {
    rx=0; 
    if( rotationAngle==0. )
    {
      rx(I,xAxis,xAxis)=1./(xb[0]-xa[0]); 
      rx(I,yAxis,yAxis)=1./(xb[1]-xa[1]); 
      rx(I,zAxis,zAxis)=1./(xb[2]-xa[2]); 
    }
    else
    {
      const int dir1 = (rotationAxis+1) % 3;
      const int dir2 = (rotationAxis+2) % 3;
      const real theta=rotationAngle*Pi/180.;
      const real cosTheta=cos(theta);
      const real sinTheta=sin(theta);

      rx(I,rotationAxis,rotationAxis)=1./(xb[rotationAxis]-xa[rotationAxis]);
      rx(I,dir1,dir1)= cosTheta/(xb[dir1]-xa[dir1]);
      rx(I,dir1,dir2)= sinTheta/(xb[dir1]-xa[dir1]);
      rx(I,dir2,dir1)=-sinTheta/(xb[dir2]-xa[dir2]);
      rx(I,dir2,dir2)= cosTheta/(xb[dir2]-xa[dir2]);
    }
    
  }
}




//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int BoxMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!BoxMapping",
      "set corners",
      "rotate",
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
      "set corners        : Specify the corners of the box",
      "rotate             : rotate about a coordinate direction",
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

  bool plotObject=true;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  gi.appendToTheDefaultPrompt("box>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="set corners" ) 
    {
      gi.inputString(line,sPrintF(buff,
         "Enter xa,xb, ya,yb, za,zb (default=[%6.2e,%6.2e]x[%6.2e,%6.2e]x[%6.2e,%6.2e]): ",
          xa[0],xb[0],xa[1],xb[1],xa[2],xb[2]));
      if( line!="" ) sScanF(line,"%e %e %e %e %e %e ",&xa[0],&xb[0],&xa[1],&xb[1],&xa[2],&xb[2]);
      mappingHasChanged();
    }
    else if( answer=="specify corners" ) // old way
    {
      gi.inputString(line,sPrintF(buff,
         "Enter xa,ya,za, xb,yb,zb (default=(%6.2e,%6.2e,%6.2e,%6.2e,%6.2e,%6.2e)): ",
          xa[0],xa[1],xa[2],xb[0],xb[1],xb[2]));
      if( line!="" ) sScanF(line,"%e %e %e %e %e %e  ",&xa[0],&xa[1],&xa[2],&xb[0],&xb[1],&xb[2]);
      mappingHasChanged();
    }
    else if( answer=="rotate" ) 
    {
      gi.inputString(line,sPrintF(buff,"Enter rotation angle(degrees) and axis to rotate about(0,1, or 2)"
				  "(default=(%e,%i)): ",rotationAngle,rotationAxis));
      if( line!="" ) sScanF(line,"%e %i",&rotationAngle,&rotationAxis);
      if( rotationAxis<0 || rotationAxis>2 )
      {
	cout << "Invalid rotation axis = " << rotationAxis << endl;
        rotationAxis=2;
	continue;
      }
      gi.inputString(line,sPrintF(buff,"Enter the point to rotate around (default=%e,%e,%e): ",
				  centerOfRotation[0],centerOfRotation[1],centerOfRotation[2]));
      if( line!="" ) sScanF(line,"%e %e %e",&centerOfRotation[0],&centerOfRotation[1],
			    &centerOfRotation[2]);

      rotate( rotationAngle,rotationAxis,centerOfRotation[0],centerOfRotation[1],centerOfRotation[2] );
      mappingHasChanged();
    }
    else if( answer=="show parameters" )
    {
      printf(" (xa,ya,za,xb,yb,zb)=(%e,%e,%e,%e,%e,%e)\n",xa[0],xa[1],xa[2],xb[0],xb[1],xb[2]);
      printf("rotation angle = %e, rotationAxis=%i, center=(%6.2e,%6.2e,%6.2e)\n",
             rotationAngle,rotationAxis,centerOfRotation[0],centerOfRotation[1],centerOfRotation[2]);
      display();
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi, *this, parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
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
      plotObject=true;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }

    if( plotObject )
    {
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi, *this, parameters);   // *** recompute every time ?? ***

    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
}
