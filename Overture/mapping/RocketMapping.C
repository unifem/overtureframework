#include "Geom.h"
#include "RocketMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "display.h"

//#include <float.h>

RocketMapping::
RocketMapping(const int & rangeDimension_ /* =2 */ ) 
   : SplineMapping(rangeDimension_)
//===========================================================================
/// \brief 
///     Define various cross-sections related to rocket geometries
/// 
/// \param rangeDimension_ : 2, 3
/// 
/// \param Author: Nathan Crane, cleaned up by Bill Henshaw.
//===========================================================================
{ 
  RocketMapping::className="RocketMapping";
  setName( Mapping::mappingName,"RocketMapping");

  numVertex=3;
  elementSize=0.1;
  // slot parameters:
  innerRadius=1.0;
  outerRadius=3.0;
  slotWidth=0.5;
  zValue=0.0;
  numPoints=0;
  
  // star parameters:
  innerFilletRadius=0.1;
  outerFilletRadius=0.2;

  // for circle:
  radius=1.0;

  rocketType=slot;
}

// Copy constructor is deep by default
RocketMapping::
RocketMapping( const RocketMapping & map, const CopyType copyType )
{
  RocketMapping::className="RocketMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "RocketMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

RocketMapping::
~RocketMapping()
{ if( debug & 4 )
  cout << " RocketMapping::Desctructor called" << endl;
}

RocketMapping & RocketMapping::
operator=( const RocketMapping & X )
{
  if( RocketMapping::className != X.getClassName() )
  {
    cout << "RocketMapping::operator= ERROR trying to set a RocketMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->SplineMapping::operator=(X);            // call = for derivee class

  numVertex=X.numVertex;
  elementSize=X.elementSize;
  innerRadius=X.innerRadius;
  outerRadius=X.outerRadius;
  slotWidth=X.slotWidth;
  zValue=X.zValue;
  numPoints=X.numPoints;
  innerFilletRadius=X.innerFilletRadius;
  outerFilletRadius=X.outerFilletRadius;
  radius=X.radius;
  rocketType=X.rocketType;

  return *this;
}


int RocketMapping::
computePoints()
//=====================================================================================
/// \brief  
///    Compute the spline points.
//=====================================================================================
{
  if( rocketType==slot )
    computeSlotPoints();
  else if( rocketType==star )    
    computeStarPoints();
  else
    computeCirclePoints();

  initialized=false;   
  mappingHasChanged();
  return 0;
}



int RocketMapping::
computeSlotPoints()
//=====================================================================================
/// \brief  Compute the spline points for a slotted cross-section.
//=====================================================================================
{
//
//              Compute critical circle centers for outer 12:00 circle and inner circle
//		one position clockwise from the 12:00 circle
//
  Real ocentrad=outerRadius-slotWidth/2.0; 
  Real theta;
  RealArray ocirccent(2);
  RealArray icirccent(2);
//
//		Calculate portions of inner circle taken up by slot
//
  real side_theta, slot_theta, tot_theta;

  tot_theta=2.0*M_PI/numVertex;
  slot_theta=2.0*asin(slotWidth/(2.0*innerRadius));
  side_theta=(tot_theta-slot_theta);
//
//              Compute number of points required in each slot section
//
//  real tot_arclen=side_theta*innerRadius+2*(outerRadius-innerRadius)+M_PI*slotWidth/2.0;

  float arclen;
  
  int end_points, side_points, line_points;

  arclen=side_theta*innerRadius;
  side_points=int(fabs(rounder(arclen/elementSize)));

  arclen=M_PI*slotWidth/2.0;
  end_points=int(fabs(rounder(arclen/elementSize)));
  if(end_points%2 != 0) end_points=end_points+1;

  arclen=outerRadius-innerRadius;
  line_points=rounder(arclen/elementSize);
//
//		Assign varibles for point creation loops
//
  int curpoint=0;
  int ivertex, ipoint;
  int tot_points=(side_points+end_points+2*line_points)*numVertex;
  numberOfSplinePoints=tot_points;

  knots.redim(tot_points,rangeDimension);
  RealArray start_line(2), end_line(2);
  real centx, centy, basetheta;
//
//		Loop over each vertex creating the required slot portions
//
  for(ivertex=0;ivertex<numVertex;ivertex++)
    {
    basetheta=(((double)ivertex)/numVertex)*2*M_PI;
    centx=ocentrad*cos(basetheta);
    centy=ocentrad*sin(basetheta);
//
//              Draw half of outer circle
//
    for(ipoint=0;ipoint<end_points/2;ipoint++) {
      theta=basetheta+(M_PI)*(double)ipoint/end_points;
      knots(curpoint,0)=centx+(slotWidth/2.0)*cos(theta);
      knots(curpoint,1)=centy+(slotWidth/2.0)*sin(theta);
      curpoint++;
    }
//
//              Draw Line portion 1
//   
    start_line(0)=centx+(slotWidth/2.0)*cos(basetheta+M_PI/2.0);
    start_line(1)=centy+(slotWidth/2.0)*sin(basetheta+M_PI/2.0);
    
    end_line(0)=innerRadius*cos(basetheta+slot_theta/2.0);
    end_line(1)=innerRadius*sin(basetheta+slot_theta/2.0);
    
    for(ipoint=0;ipoint<line_points;ipoint++) {
      knots(curpoint,0)=start_line(0)*
                         (1.0-(double)ipoint/line_points)
                         +end_line(0)*((double)ipoint/line_points);
      knots(curpoint,1)=start_line(1)*
                         (1.0-(double)ipoint/line_points)
                         +end_line(1)*((double)ipoint/line_points);
      curpoint++;
    }
//
//              Draw inner circle 
//    
    for(ipoint=0;ipoint<side_points;ipoint++) {
      theta=basetheta+slot_theta/2.0+(side_theta)*(double)ipoint/side_points;
      knots(curpoint,0)=innerRadius*cos(theta);
      knots(curpoint,1)=innerRadius*sin(theta);
      curpoint++;
    }
//
//              Draw Line 2
//
    centx=ocentrad*cos(basetheta+tot_theta);
    centy=ocentrad*sin(basetheta+tot_theta);
    start_line(0)=innerRadius*cos(basetheta+slot_theta/2.0+side_theta);
    start_line(1)=innerRadius*sin(basetheta+slot_theta/2.0+side_theta);
    end_line(0)=centx+(slotWidth/2.0)*cos(basetheta+tot_theta-M_PI/2.0);
    end_line(1)=centy+(slotWidth/2.0)*sin(basetheta+tot_theta-M_PI/2.0);
    
    for(ipoint=0;ipoint<line_points;ipoint++) {
      knots(curpoint,0)=start_line(0)*
                         (1.0-(double)ipoint/line_points)
                         +end_line(0)*((double)ipoint/line_points);
      knots(curpoint,1)=start_line(1)*
                         (1.0-(double)ipoint/line_points)
                         +end_line(1)*((double)ipoint/line_points);
      curpoint++;
    }
//
//              Draw half of outer circle
//
    for(ipoint=0;ipoint<end_points/2;ipoint++) {
      theta=basetheta+tot_theta-M_PI/2.0+(M_PI)*(double)ipoint/end_points;
      knots(curpoint,0)=centx+(slotWidth/2.0)*cos(theta);
      knots(curpoint,1)=centy+(slotWidth/2.0)*sin(theta);
      curpoint++;
    }
  }

  if( rangeDimension==3 )
  {
    Range all;
    knots(all,2)=zValue;
  }
//
//              Compute total length
//   
  real tot_len=0.0;
  real cur_len;
  for(ipoint=1;ipoint<curpoint;ipoint++) {
    cur_len=sqrt(pow((knots(ipoint,0)-knots(ipoint-1,0)),2.)+
                 pow((knots(ipoint,1)-knots(ipoint-1,1)),2.));  // *wdh* changed pow(...,1) to pow(...,2.)
    tot_len=tot_len+cur_len;
    }
  tot_points=curpoint;
//
//		Set number of lines and periodicity
//
  setGridDimensions(0,tot_points);
  isPeriodic[0]=functionPeriodic;
  splineIsPeriodic=getIsPeriodic(axis1);
  bc[0][0]=-1;
  bc[1][0]=-1;
//
//		Successful completion
//
  return 0;
}  

int RocketMapping::
computeStarPoints()
//=====================================================================================
/// \brief  Supply spline points for a star cross-section.
//=====================================================================================
{
//
//              Compute critical circle centers for outer 12:00 circle and inner circle
//		one position clockwise from the 12:00 circle
//
  

  Real ocentrad=outerRadius-outerFilletRadius; 
  Real icentrad=innerRadius+innerFilletRadius;

  Real theta;

  realArray ocirccent(2);
  realArray icirccent(2);

  theta=M_PI/2.0;
  ocirccent(0)=cos(theta)*ocentrad;
  ocirccent(1)=sin(theta)*ocentrad;

  theta=M_PI/2.0-(2*M_PI)*(1.0/(numVertex*2.0));
  icirccent(0)=cos(theta)*icentrad;
  icirccent(1)=sin(theta)*icentrad;
//
//		Determine tangential line intersection of the two circles using a robust
//		though highly innefficent method by sampling a range of thetas for the
//		optimum value
//
  real itheta=0.0;  
  realArray x1(2), v1(2), v2(2), v3(2), v4(2);
  real test_rad, temp_rad;

  x1(0)=ocirccent(0)+outerFilletRadius*cos(itheta);
  x1(1)=ocirccent(1)+outerFilletRadius*sin(itheta);
  v1(0)=sin(itheta);
  v1(1)=-cos(itheta);  
  v2(0)=icirccent(0)-x1(0);
  v2(1)=icirccent(1)-x1(1);
  v3(0)=v1(0)*(v1(0)*v2(0)+v1(1)*v2(1));
  v3(1)=v1(1)*(v1(0)*v2(0)+v1(1)*v2(1));
  v4(0)=v2(0)-v3(0);
  v4(1)=v2(1)-v3(1);
  test_rad=sqrt(pow(v4(0),2.)+pow(v4(1),2.))-innerFilletRadius;
  for(itheta=0.0;itheta<=M_PI/2.0;itheta=itheta+0.001) {
    x1(0)=ocirccent(0)+outerFilletRadius*cos(itheta);
    x1(1)=ocirccent(1)+outerFilletRadius*sin(itheta);
  
    v1(0)=sin(itheta);
    v1(1)=-cos(itheta);
  
    v2(0)=icirccent(0)-x1(0);
    v2(1)=icirccent(1)-x1(1);
  
    v3(0)=v1(0)*(v1(0)*v2(0)+v1(1)*v2(1));
    v3(1)=v1(1)*(v1(0)*v2(0)+v1(1)*v2(1));

    v4(0)=v2(0)-v3(0);
    v4(1)=v2(1)-v3(1);
  
    temp_rad=sqrt(pow(v4(0),2.)+pow(v4(1),2.))-innerFilletRadius;
  
    if((temp_rad<=0.0 && test_rad>=0.0) || (temp_rad>=0.0 && test_rad<=0.0)) break;
  
    test_rad=temp_rad;
  }
//
//              Find idtheta and odtheta, the angle at which the tangency point
//		Intersects the two circles
//
  real odtheta=M_PI-itheta*2.0;
  real deltax=fabs(icirccent(0)-ocirccent(0));
  real deltay=fabs(icirccent(1)-ocirccent(1));
  real idtheta=odtheta-atan2((double)deltay,(double)deltax);    
//
//              Compute number of points required in each star section
//
  real tot_arclen=fabs(odtheta*outerFilletRadius)+
                 fabs(idtheta*innerFilletRadius)+
                 2*sqrt(pow(v3(0),2.)+pow(v3(1),2.));

  float arclen;
  
  int out_points, in_points, line_points;
  int points_per_vertex;

  if(numPoints==0)
  {
    arclen=fabs(odtheta*outerFilletRadius);
    out_points=int(fabs(rounder(arclen/elementSize)));
    if(out_points % 2 != 0) out_points=out_points+1;

    arclen=fabs(idtheta*innerFilletRadius);
    in_points=int(fabs(rounder(arclen/elementSize)));

    arclen=sqrt(pow(v3(0),2.)+pow(v3(1),2.));
    line_points=rounder(arclen/elementSize);
  }
  else {
    points_per_vertex=numPoints/numVertex;
    arclen=fabs(odtheta*outerFilletRadius);
    out_points=int((arclen/tot_arclen)*points_per_vertex);
    if(out_points % 2 != 0) out_points=out_points+1;

    arclen=fabs(idtheta*innerFilletRadius);
    in_points=int((arclen/tot_arclen)*points_per_vertex);

    arclen=sqrt(pow(v3(0),2.)+pow(v3(1),2.));
    line_points=int((arclen/tot_arclen)*points_per_vertex);

    in_points=in_points+(points_per_vertex-(in_points+out_points+2*line_points));
  }
//
//		Assign varibles for point creation loops
//
  int curpoint=0;
  int ivertex, ipoint;
  int tot_points=(out_points+in_points+2*line_points)*numVertex;

//
//	Set up knot array
//
  numberOfSplinePoints=tot_points;
 //  printf("Star: numberOfSplinePoints=%i\n",numberOfSplinePoints);
  
  knots.redim(tot_points,rangeDimension);

  realArray start_line(2), end_line(2);
  real centx, centy, basetheta, rad;
//
//		Loop over each vertex creating the required star portions
//
  for(ivertex=0;ivertex<numVertex;ivertex++)
    {
    basetheta=(((double)ivertex)/numVertex)*2*M_PI;
    centx=ocentrad*cos(basetheta);
    centy=ocentrad*sin(basetheta);
//
//              Draw half of outer circle
//
    for(ipoint=0;ipoint<out_points/2;ipoint++) {
      theta=basetheta+(odtheta)*(double)ipoint/out_points;
      knots(curpoint,0)=centx+outerFilletRadius*cos(theta);
      knots(curpoint,1)=centy+outerFilletRadius*sin(theta);
      // *wdh* knots(curpoint,2)=zValue;
      curpoint++;
    }
//
//              Draw Line portion 1
//   
    basetheta=(((double)ivertex)/numVertex)*2*M_PI;
    centx=ocentrad*cos(basetheta);
    centy=ocentrad*sin(basetheta);
    start_line(0)=centx+outerFilletRadius*cos(basetheta+(odtheta/2.0));
    start_line(1)=centy+outerFilletRadius*sin(basetheta+(odtheta/2.0));
    
    basetheta=(((double)ivertex+0.5)/numVertex)*2*M_PI;
    centx=icentrad*cos(basetheta);
    centy=icentrad*sin(basetheta);
    end_line(0)=centx+innerFilletRadius*cos(basetheta+M_PI+idtheta/2.0);
    end_line(1)=centy+innerFilletRadius*sin(basetheta+M_PI+idtheta/2.0);
    
    for(ipoint=0;ipoint<line_points;ipoint++) {
      knots(curpoint,0)=start_line(0)*
                         (1.0-(double)ipoint/line_points)
                         +end_line(0)*((double)ipoint/line_points);
      knots(curpoint,1)=start_line(1)*
                         (1.0-(double)ipoint/line_points)
                         +end_line(1)*((double)ipoint/line_points);
      // *wdh* knots(curpoint,2)=zValue;
      curpoint++;
    }
//
//              Draw inner circle 
//    
    basetheta=(((double)ivertex+0.5)/numVertex)*2*M_PI;
    centx=icentrad*cos(basetheta);
    centy=icentrad*sin(basetheta);  
    for(ipoint=0;ipoint<in_points;ipoint++) {
      theta=(basetheta+M_PI)+(idtheta/2.0)-idtheta*(double)ipoint/in_points;
      knots(curpoint,0)=centx+innerFilletRadius*cos(theta);
      knots(curpoint,1)=centy+innerFilletRadius*sin(theta);
      // *wdh* knots(curpoint,2)=zValue;
      curpoint++;
    }
//
//              Draw Line 2
//
    basetheta=(((double)ivertex+0.5)/numVertex)*2*M_PI;
    centx=icentrad*cos(basetheta);
    centy=icentrad*sin(basetheta);
    start_line(0)=centx+innerFilletRadius*
                  cos(basetheta+M_PI+idtheta/2.0-idtheta);
    start_line(1)=centy+innerFilletRadius*
                  sin(basetheta+M_PI+idtheta/2.0-idtheta);
    basetheta=(((double)ivertex+1.0)/numVertex)*2*M_PI;
    centx=ocentrad*cos(basetheta);
    centy=ocentrad*sin(basetheta);
    end_line(0)=centx+outerFilletRadius*cos(basetheta-(odtheta/2.0));
    end_line(1)=centy+outerFilletRadius*sin(basetheta-(odtheta/2.0));
    
    for(ipoint=0;ipoint<line_points;ipoint++) {
      knots(curpoint,0)=start_line(0)*
                         (1.0-(double)ipoint/line_points)
                         +end_line(0)*((double)ipoint/line_points);
      knots(curpoint,1)=start_line(1)*
                         (1.0-(double)ipoint/line_points)
                         +end_line(1)*((double)ipoint/line_points);
      // *wdh* knots(curpoint,2)=zValue;
      curpoint++;
    }
//
//              Draw half of outer circle
//
    basetheta=(((double)ivertex+1.0)/numVertex)*2*M_PI;  
    centx=ocentrad*cos(basetheta);
    centy=ocentrad*sin(basetheta);
    for(ipoint=0;ipoint<out_points/2;ipoint++) {
      theta=basetheta-(odtheta/2.0)+(odtheta)*(double)ipoint/out_points;
      knots(curpoint,0)=centx+outerFilletRadius*cos(theta);
      knots(curpoint,1)=centy+outerFilletRadius*sin(theta);
      // *wdh* knots(curpoint,2)=zValue;
      curpoint++;
    }
  }
//
//              Enforce radius constraints
//
  for(ipoint=0;ipoint<curpoint;ipoint++) {
    rad=sqrt(pow(knots(ipoint,0),2.)+pow(knots(ipoint,1),2.));
    if(rad>outerRadius) {
      knots(ipoint,0)=knots(ipoint,0)*outerRadius/rad;
      knots(ipoint,1)=knots(ipoint,1)*outerRadius/rad;
      }
    if(rad<innerRadius) {
      knots(ipoint,0)=knots(ipoint,0)*innerRadius/rad;
      knots(ipoint,1)=knots(ipoint,1)*innerRadius/rad;
    }
  }

  if( rangeDimension==3 )
  {
    Range all;
    knots(all,2)=zValue;
  }
  
//
//              Compute total length
//   
  real tot_len=0.0;
  real cur_len;
  for(ipoint=1;ipoint<curpoint;ipoint++) {
    cur_len=sqrt(pow((knots(ipoint,0)-knots(ipoint-1,0)),2.)+
            pow((knots(ipoint,1)-knots(ipoint-1,1)),2.));
    tot_len=tot_len+cur_len;
    }
  tot_points=curpoint;
//
//              Compute optimum normal distance for mesh
//
  real norm1=-outerFilletRadius*0.55;
  real norm2=-innerFilletRadius*2.2;
  if(fabs(norm1)< fabs(norm2)) normalDistance=norm1;
  if(fabs(norm2)<=fabs(norm1)) normalDistance=norm2;
//
//		Set number of lines and periodicity
//
  setGridDimensions(0,tot_points);
  isPeriodic[0]=functionPeriodic;
  splineIsPeriodic=getIsPeriodic(axis1);
  bc[0][0]=-1;
  bc[1][0]=-1;

//
//		Successful completion
//
  return 0;
}  


int RocketMapping::
computeCirclePoints()
//=====================================================================================
/// \brief  Supply spline points for a 2D Circ.
//=====================================================================================
{
  real theta;
  int circ_points;
//
//
//              Compute number of points required in each Circ section
//
  real tot_arclen=2*M_PI*radius;


  if(numPoints==0) {
    circ_points=rounder(tot_arclen/elementSize);
  }
  else {
    circ_points=numPoints;
  }
//
//		Assign varibles for point creation loops
//
  int curpoint=0;
  int ipoint;
//
//	Set up knot array
//
  numberOfSplinePoints=circ_points;
  knots.redim(circ_points,rangeDimension);
//
//		Loop over each vertex creating the required star portions
//
  for(ipoint=0;ipoint<circ_points;ipoint++) {
    theta=(2*M_PI)*(double)ipoint/circ_points;
    knots(curpoint,0)=radius*cos(theta);
    knots(curpoint,1)=radius*sin(theta);
    knots(curpoint,2)=zValue;
    curpoint++;
  }
//
//		Set number of lines and periodicity
//
  setGridDimensions(0,circ_points);
  isPeriodic[0]=functionPeriodic;
  splineIsPeriodic=getIsPeriodic(axis1);
  bc[0][0]=-1;
  bc[1][0]=-1;
//
//		Successful completion
//
  return 0;
}  


//=================================================================================
// get a mapping from the database
//=================================================================================
int RocketMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");

  if( debug & 4 )
    cout << "Entering RocketMapping::get" << endl;

  subDir.get( RocketMapping::className,"className" ); 
  if( RocketMapping::className != "RocketMapping" )
  {
    cout << "RocketMapping::get ERROR in className!" << endl;
  }

  subDir.get( numVertex,"numVertex" );
  subDir.get( elementSize,"elementSize" );
  subDir.get( innerRadius,"innerRadius" );
  subDir.get( outerRadius,"outerRadius" );
  subDir.get( slotWidth,"slotWidth" );
  subDir.get( zValue,"zValue" );
  subDir.get( numPoints,"numPoints" );
  subDir.get( innerFilletRadius,"innerFilletRadius" );
  subDir.get( outerFilletRadius,"outerFilletRadius" );
  subDir.get( radius,"radius" );
  int temp;
  subDir.get( temp,"rocketType" ); rocketType=(RocketTypeEnum)temp;

  SplineMapping::get( subDir, "SplineMapping" );
  delete &subDir;
  
  mappingHasChanged();
  return 0;
}

int RocketMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( RocketMapping::className,"className" );

  subDir.put( numVertex,"numVertex" );
  subDir.put( elementSize,"elementSize" );
  subDir.put( innerRadius,"innerRadius" );
  subDir.put( outerRadius,"outerRadius" );
  subDir.put( slotWidth,"slotWidth" );
  subDir.put( zValue,"zValue" );
  subDir.put( numPoints,"numPoints" );
  subDir.put( innerFilletRadius,"innerFilletRadius" );
  subDir.put( outerFilletRadius,"outerFilletRadius" );
  subDir.put( radius,"radius" );
  subDir.put( (int)rocketType,"rocketType" );


  SplineMapping::put( subDir, "SplineMapping" );
  delete &subDir;
  return 0;
}

Mapping *RocketMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==RocketMapping::className )
    retval = new RocketMapping();
  return retval;
}

    

int RocketMapping::
update( MappingInformation & mapInfo ) 
//=====================================================================================
/// \brief  Interactively create and/or change the spline mapping.
/// \param mapInfo (input): Holds a graphics interface to use.
//=====================================================================================
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!RocketMapping",
      ">rocket type",
        "slot",
        "star",
        "circle",
      "<>slot parameters",
        "set bounding radii",
        "set slot width",
        "set element size",
        "set number of vertices",
      "<>star parameters",
        "set bounding radii",
        "set fillet radii",
        "set element size",
        "set number of vertices",
        "set number of points",
      "<>circle parameters",
        "set radius",
      "<set range dimension",
      "set z value",
      "change spline parameters",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "check",
      "show spline outside boundaries",
      "show parameters",
      "check mapping",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "set range dimension: 2, or 3 for a 2D or 3D curve",
      "set bounding radii : set inner and outer bound radius for slot",
      "set slot width : Set the width of slots",
      " ",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "check              : check properties of this mapping",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,line,answer2; 

  if( !initialized )
    computePoints();

  bool plotObject=true; // numberOfSplinePoints>0;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  bool showSplineOutsideBoundaries=FALSE;

  gi.appendToTheDefaultPrompt("Spline>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);

    if( answer=="slot" )
    {
      rocketType=slot;
      computePoints();
    }
    else if( answer=="star" )
    {
      rocketType=star;
      computePoints();
      //numVertex=7;
      //numPoints=0;
      // rangeDimension=3;
    }
    else if( answer=="circle" )
    {
      rocketType=circle;
      computePoints();
    }
    else if( answer=="set range dimension" )
    {
      gi.inputString(line,sPrintF(buff,"Enter range dimension: 1,2, or 3 (current=%i)",rangeDimension));
      if( line!="" )
      {
        sScanF(line,"%i ",&rangeDimension);
        rangeDimension=max(1,min(3,rangeDimension));
        computePoints();
      }
    }
    else if( answer=="set bounding radii" )
    {
      gi.inputString(line,sPrintF(buff,"Enter inner and outer radii, inner=%6.2e, outer=%6.2e) :",
                     innerRadius, outerRadius));
      if( line!="" )
      {
        sScanF(line,"%e %e",&innerRadius,&outerRadius);
        printf("RocketMapping:INFO Setting the radii to be inner=%6.2e, outer=%6.2e \n",
                innerRadius, outerRadius);
        computePoints();
      }
    }
    else if( answer=="set z value" )
    {
      gi.inputString(line,sPrintF(buff,"Enter star z value = %6.2e) :",zValue));
      if( line!="" )
      {
        sScanF(line,"%e",&zValue);
        printf("RocketMapping:INFO Setting z value = %6.2e \n",zValue);
        computePoints();
      }
    }
    else if( answer=="set slot width" )
    {
      gi.inputString(line,sPrintF(buff,"Enter Slot Width = %6.2e) :",slotWidth));
      if( line!="" )
      {
        sScanF(line,"%e",&slotWidth);
        printf("RocketMapping:INFO Setting the slot width to %6.2e \n",
                slotWidth);
        computePoints();
      }
    }
    else if( answer=="set number of vertices" )
    {
      gi.inputString(line,sPrintF(buff,"Enter number of vertices, %6i) :",
                     numVertex));
      if( line!="" )
      {
        sScanF(line,"%i",&numVertex);
        printf("RocketMapping:INFO Setting number of vertices to %i \n",
                numVertex);
        computePoints();
      }
    }
    else if( answer=="set number of points" )
    {
      gi.inputString(line,sPrintF(buff,"Enter number of points, %6i) :",numPoints));
      if( line!="" )
      {
        sScanF(line,"%i",&numPoints);
        printf("RocketMapping:INFO Setting number of points to %i \n",
                numPoints);
        computePoints();
      }
    }
    else if( answer=="set element size" )
    {
      gi.inputString(line,sPrintF(buff,"Enter element size, %6.2e) :",
                     elementSize));
      if( line!="" )
      {
        sScanF(line,"%e",&elementSize);
        printf("RocketMapping:INFO Setting element size to %e \n",
                elementSize);
        computePoints();
      }
    }
    else if( answer=="set fillet radii" )
    {
      gi.inputString(line,sPrintF(buff,"Enter inner and outer fillet radii, inner=%6.2e, outer=%6.2e) :",
                     innerFilletRadius, outerFilletRadius));
      if( line!="" )
      {
        sScanF(line,"%e %e",&innerFilletRadius,&outerFilletRadius);
        printf("RocketMapping:INFO Setting the fillet radii to be inner=%6.2e, outer=%6.2e \n",
                innerFilletRadius, outerFilletRadius);
        computePoints();
      }
    }
    else if( answer=="set radius" )
    {
      gi.inputString(line,sPrintF(buff,"Enter radius = %6.2e) :", radius));
      if( line!="" )
      {
        sScanF(line,"%e",&radius);
        printf("RocketMapping:INFO Setting the radius to be %6.2e\n", radius);
        computePoints();
      }
    }
    else if( answer=="change spline parameters" )
    {
      SplineMapping::update(mapInfo);
      computePoints();
    }
    else if( answer=="show parameters" )
    {
      printf(" inner radius = %6.2e \n"
             " outer radius = %6.2e \n"
             " slot width = %6.2e \n"
             " number of vertices = %i \n"
             " element size = %6.2e \n",
             innerRadius, outerRadius, slotWidth,
             numVertex, elementSize );
      SplineMapping::display();
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
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity"  ||
             answer=="check" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
      if( answer=="periodicity" )
      {
        splineIsPeriodic=getIsPeriodic(axis1);
	initialized=FALSE;
      }
    }
    else if( answer=="show spline outside boundaries" )
    {
      showSplineOutsideBoundaries=TRUE;
    }
    else if( answer=="check mapping" )
      checkMapping();
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
      parameters.set(GI_PLOT_GRID_POINTS_ON_CURVES,TRUE);
      gi.erase();
      PlotIt::plot(gi,*this,parameters);  

      parameters.set(GI_USE_PLOT_BOUNDS,TRUE); 
      if( initialized && rangeDimension==1 )
      {
	Range R(0,numberOfSplinePoints-1);
        RealArray points(numberOfSplinePoints,2);
	points(R,0)=s(R);
	points(R,1)=knots(R,0);
        #ifndef USE_PPP
	gi.plotPoints(points,parameters);
        #endif
      }
      else
      {
        #ifndef USE_PPP
          gi.plotPoints(knots,parameters);
        #endif
      }
      
      if( showSplineOutsideBoundaries )
      {
	int n = max(101,getGridDimensions(0));
	real dr = 1./max(1,n-1);
	const real a=-.05, b=1.05;
	n=int( (b-a)/dr+.5);
	Range R(0,n);
	RealArray r(R,1),x(R,3); // rangeDimension);
      
	r.seqAdd(a,dr);
	mapS(r,x);
        #ifndef USE_PPP   
	gi.plotPoints(x,parameters);
        #endif
      }

      parameters.set(GI_USE_PLOT_BOUNDS,FALSE); 
    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}
