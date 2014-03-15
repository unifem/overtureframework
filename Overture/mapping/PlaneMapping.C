#include "PlaneMapping.h"
#include "MappingInformation.h"
#include "GenericDataBase.h"

//-------------------------------------------------------------
// PlaneMapping
// The equation for a plane is defined by a point (lower left corner)
// and two 
//
//  x = (xa,ya,za) + tangent1*r1 + tangent2*r2
//
//-------------------------------------------------------------

PlaneMapping::
PlaneMapping(const real & x1 /* =0. */, const real & y1 /* =0. */, const real & z1 /* =0. */,
	     const real & x2 /* =1. */, const real & y2 /* =0. */, const real & z2 /* =0. */,
	     const real & x3 /* =0. */, const real & y3 /* =1. */, const real & z3 /* =0. */)
  : Mapping(2,3,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  Default Constructor, define a plane (or rhomboid) by three
///    (non-collinear) points, p1=(x1,y1,z1), p2=(x2,y2,z2), p3=(x3,y3,z3)
///   arranged as:
///  \begin{verbatim}
///                   p3------------
///                     |          |
///                     |          |
///                     |          |
///                     |          |
///                     -----------
///                   p1           p2
/// 
///  \end{verbatim}
//===========================================================================
{ 
  PlaneMapping::className="PlaneMapping";         
  setName( Mapping::mappingName,"plane");

  setPoints(x1,y1,z1, x2,y2,z2, x3,y3,z3 );

  setRangeBound(Start,axis1,min(x1,x2,x3));
  setRangeBound(End  ,axis1,max(x1,x2,x3));
  setRangeBound(Start,axis2,min(y1,y2,y3));
  setRangeBound(End  ,axis2,max(y1,y2,y3));
  setRangeBound(Start,axis3,min(z1,z2,z3));
  setRangeBound(End  ,axis3,max(z1,z2,z3));
  
  setInvertible( true );
  setBasicInverseOption(canInvert);    
  inverseIsDistributed=false;

  mappingHasChanged();
}


// Copy constructor is deep by default
PlaneMapping::
PlaneMapping( const PlaneMapping & map, const CopyType copyType )
{
  PlaneMapping::className="PlaneMapping";         
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "PlaneMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

PlaneMapping::
~PlaneMapping()
{ if( (Mapping::debug/4) % 2 )
     cout << " PlaneMapping::Desctructor called" << endl;
}

PlaneMapping & PlaneMapping::
operator =( const PlaneMapping & X )
{
  if( PlaneMapping::className != X.getClassName() )
  {
    cout << "PlaneMapping::operator= ERROR trying to set a PlaneMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  xa=X.xa;
  ya=X.ya;
  za=X.za;
  vector1=X.vector1;
  vector2=X.vector2;
  return *this;
}

int PlaneMapping::
setPoints(const real & x1 /* =0. */, const real & y1 /* =0. */, const real & z1 /* =0. */,
	  const real & x2 /* =1. */, const real & y2 /* =0. */, const real & z2 /* =0. */,
	  const real & x3 /* =0. */, const real & y3 /* =1. */, const real & z3 /* =0. */)
//===========================================================================
/// \brief  Set the corners of the plane or rhomboid. The plane (or rhomboid) 
///   is defined by three
///    (non-collinear) points, p1=(x1,y1,z1), p2=(x2,y2,z2), p3=(x3,y3,z3)
///   arranged as:
///  \begin{verbatim}
///                   p3------------
///                     |          |
///                     |          |
///                     |          |
///                     |          |
///                     -----------
///                   p1           p2
/// 
///  \end{verbatim}
//===========================================================================
{ 
  vector1.redim(3);
  vector2.redim(3);
  
  vector1(0)=x2-x1;
  vector1(1)=y2-y1;
  vector1(2)=z2-z1;

  vector2(0)=x3-x1;
  vector2(1)=y3-y1;
  vector2(2)=z3-z1;
  
  xa=x1; ya=y1; za=z1;

  v1DotV1=vector1(0)*vector1(0)+vector1(1)*vector1(1)+vector1(2)*vector1(2);
  v1DotV2=vector1(0)*vector2(0)+vector1(1)*vector2(1)+vector1(2)*vector2(2);
  v2DotV2=vector2(0)*vector2(0)+vector2(1)*vector2(1)+vector2(2)*vector2(2);

  real det = v1DotV1*v2DotV2-v1DotV2*v1DotV2;
  if( det==0. )
  {
    cout << "PlaneMapping::setPoints:ERROR the three points are collinear! \n";
    return 1;
  }
  // divide through by det for inverseMap computation
  v1DotV1/=det;
  v1DotV2/=det;
  v2DotV2/=det;
  

  mappingHasChanged();
  return 0;
}


// ---get a mapping from the database---
int PlaneMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( (Mapping::debug/4) % 2 )
    cout << "Entering PlaneMapping::get" << endl;

  subDir.get( PlaneMapping::className,"className" ); 
  if( PlaneMapping::className != "PlaneMapping" )
  {
    cout << "PlaneMapping::get ERROR in className!" << endl;
  }
  subDir.get( xa,"xa" );
  subDir.get( ya,"ya" );
  subDir.get( za,"za" );

  subDir.get( v1DotV1,"v1DotV1" );
  subDir.get( v1DotV2,"v1DotV2" );
  subDir.get( v2DotV2,"v2DotV2" );

  subDir.get( vector1,"vector1" );
  subDir.get( vector2,"vector2" );
  Mapping::get( subDir, "Mapping" );
  mappingHasChanged();
  delete & subDir;
  return 0;
}

int PlaneMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( PlaneMapping::className,"className" );
  subDir.put( xa,"xa" );
  subDir.put( ya,"ya" );
  subDir.put( za,"za" );

  subDir.put( v1DotV1,"v1DotV1" );
  subDir.put( v1DotV2,"v1DotV2" );
  subDir.put( v2DotV2,"v2DotV2" );

  subDir.put( vector1,"vector1" );
  subDir.put( vector2,"vector2" );
  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping* PlaneMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=NULL;
  if( mappingClassName==PlaneMapping::className )
  {
    retval = new PlaneMapping();
    assert( retval != 0 );
  }
  return retval;
}

void PlaneMapping::
map( const realArray & r, realArray & x, realArray & xr,
                      MappingParameters & params )
{ 
  if( params.coordinateType != cartesian )
    cerr << "PlaneMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    x(I,axis1)=xa+vector1(axis1)*r(I,axis1) + vector2(axis1)*r(I,axis2); 
    x(I,axis2)=ya+vector1(axis2)*r(I,axis1) + vector2(axis2)*r(I,axis2); 
    x(I,axis3)=za+vector1(axis3)*r(I,axis1) + vector2(axis3)*r(I,axis2);
  }
  if( computeMapDerivative )
  {
    xr(I,axis1,axis1)=vector1(axis1);
    xr(I,axis1,axis2)=vector2(axis1);
    xr(I,axis2,axis1)=vector1(axis2);
    xr(I,axis2,axis2)=vector2(axis2);
    xr(I,axis3,axis1)=vector1(axis3);
    xr(I,axis3,axis2)=vector2(axis3);
  }
}

void PlaneMapping::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    realArray v1DotX(I);
    realArray v2DotX(I);

    v1DotX(I)=(x(I,0)-xa)*vector1(0)+(x(I,1)-ya)*vector1(1)+(x(I,2)-za)*vector1(2);
    v2DotX(I)=(x(I,0)-xa)*vector2(0)+(x(I,1)-ya)*vector2(1)+(x(I,2)-za)*vector2(2);

    r(I,axis1)=( v2DotV2*v1DotX - v1DotV2*v2DotX );
    r(I,axis2)=( v1DotV1*v2DotX - v1DotV2*v1DotX );
  }
  if( computeMapDerivative )
  {
    rx(I,axis1,axis1)=v2DotV2*vector1(0)-v1DotV2*vector2(0);
    rx(I,axis1,axis2)=v2DotV2*vector1(1)-v1DotV2*vector2(1);
    rx(I,axis1,axis3)=v2DotV2*vector1(2)-v1DotV2*vector2(2);

    rx(I,axis2,axis1)=v1DotV1*vector2(0)-v1DotV2*vector1(0);
    rx(I,axis2,axis2)=v1DotV1*vector2(1)-v1DotV2*vector1(1);
    rx(I,axis2,axis3)=v1DotV1*vector2(2)-v1DotV2*vector1(2);
    

  }
}


void PlaneMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr,
                      MappingParameters & params )
{ 
  if( params.coordinateType != cartesian )
    cerr << "PlaneMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    x(I,axis1)=xa+vector1(axis1)*r(I,axis1) + vector2(axis1)*r(I,axis2); 
    x(I,axis2)=ya+vector1(axis2)*r(I,axis1) + vector2(axis2)*r(I,axis2); 
    x(I,axis3)=za+vector1(axis3)*r(I,axis1) + vector2(axis3)*r(I,axis2);
  }
  if( computeMapDerivative )
  {
    xr(I,axis1,axis1)=vector1(axis1);
    xr(I,axis1,axis2)=vector2(axis1);
    xr(I,axis2,axis1)=vector1(axis2);
    xr(I,axis2,axis2)=vector2(axis2);
    xr(I,axis3,axis1)=vector1(axis3);
    xr(I,axis3,axis2)=vector2(axis3);
  }
}

void PlaneMapping::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    RealArray v1DotX(I);
    RealArray v2DotX(I);

    v1DotX(I)=(x(I,0)-xa)*vector1(0)+(x(I,1)-ya)*vector1(1)+(x(I,2)-za)*vector1(2);
    v2DotX(I)=(x(I,0)-xa)*vector2(0)+(x(I,1)-ya)*vector2(1)+(x(I,2)-za)*vector2(2);

    r(I,axis1)=( v2DotV2*v1DotX - v1DotV2*v2DotX );
    r(I,axis2)=( v1DotV1*v2DotX - v1DotV2*v1DotX );
  }
  if( computeMapDerivative )
  {
    rx(I,axis1,axis1)=v2DotV2*vector1(0)-v1DotV2*vector2(0);
    rx(I,axis1,axis2)=v2DotV2*vector1(1)-v1DotV2*vector2(1);
    rx(I,axis1,axis3)=v2DotV2*vector1(2)-v1DotV2*vector2(2);

    rx(I,axis2,axis1)=v1DotV1*vector2(0)-v1DotV2*vector1(0);
    rx(I,axis2,axis2)=v1DotV1*vector2(1)-v1DotV2*vector1(1);
    rx(I,axis2,axis3)=v1DotV1*vector2(2)-v1DotV2*vector1(2);
    

  }
}




//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int PlaneMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!PlaneMapping",
      "specify plane or rhombus by three points",
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
      "specify plane or rhombus by three points",
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

  gi.appendToTheDefaultPrompt("Plane>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="specify plane or rhombus by three points" ) 
    {
      printf("A plane (or rhombus) is defined by 3 points. Choose the points in the order\n"
	     "  x1,y1,z1 : lower left corner       3-----X \n"
	     "  x2,y2,z2 : lower right corner      |     | \n"
	     "  x3,y3,z3 : upper left corner       1-----2 \n");
      real  x1,y1,z1, x2,y2,z2, x3,y3,z3;
      x1=xa;            y1=ya;            z1=za; 
      x2=xa+vector1(0); y2=ya+vector1(1); z2=za+vector1(2); 
      x3=xa+vector2(0); y3=ya+vector2(1); z3=za+vector2(2); 
      
      gi.inputString(line,sPrintF(buff,
         "Enter x1,y1,z1, x2,y2,z2, x3,y3,z3 (default=(%6.2e,%6.2e,%6.2e) ,(%6.2e,%6.2e,%6.2e),(%6.2e,%6.2e,%6.2e) ): ",
          x1,y1,z1, x2,y2,z2, x3,y3,z3));
      if( line!="" )
      {
        sScanF(line,"%e %e %e %e %e %e %e %e %e",&x1,&y1,&z1,&x2,&y2,&z2,&x3,&y3,&z3);
        setPoints(x1,y1,z1, x2,y2,z2, x3,y3,z3);
      }
      mappingHasChanged();
    }
    else if( answer=="show parameters" )
    {
      display();
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
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
