#include "OffsetShell.h"

//#include "GL_GraphicsInterface.h"
#include "GenericGraphicsInterface.h"
#include "MappingInformation.h"
#include "SplineMapping.h"
#include "MatrixTransform.h"
#include "DataPointMapping.h"
#include "StretchTransform.h"
#include "HyperbolicMapping.h"
#include "NormalMapping.h"

#include "display.h"

OffsetShell::
OffsetShell()
//===========================================================================
/// \details 
///     Starting from a 3D reference surface build an offset surface and joining edge surface.
///  
//===========================================================================
{
  referenceSurface=NULL;  // user defined "shell" or "plate"
  offsetSurface=NULL;      // surface offset from reference surface
  edgeSurface=NULL;       // edge surface that joins the ref surface to the offset surface
  edgeVolume=NULL;       // volume grid for the edge surface
  referenceVolume=NULL;  // volume grid on reference surface
  offsetVolume=NULL;     // volume grid on offset surface

  numberOfMatchingPoints=1; // number of points that sit on the surface before the circular arc begins.

  // rOffset[side][axis] : distance from bounndary of the curve on the unit square
  rOffset[0][0]=rOffset[1][0]=rOffset[0][1]=rOffset[1][1]=.05;
  cornerShift=1.3;   // move the corner point in by this factor
  cornerOffset=2.; // stop the straight line segment this factor from the corner

  cornerStretchExponent[0]=100.;
  cornerStretchExponent[1]=100.;
  cornerStretchExponent[2]=100.;
  cornerStretchExponent[3]=100.;
  

  offsetType=normalOffset;
  shift[0]=0.;
  shift[1]=0.;
  shift[2]=.2;

  offsetDistance=.2;  // for normal offset.

  numberOfEdgeLines[0]=65;
  numberOfEdgeLines[1]=11;
  numberOfEdgeLines[2]=5;
  
  normalDistance=.1;

}

OffsetShell::
~OffsetShell()
{
}



int
buildVolumeGrid( HyperbolicMapping *& hypeVolume, Mapping & edgeSurface,
                 int linesToMarch, real distanceToMarch, 
                 MappingInformation & mapInfo )
// =========================================================================
// /Description:
//    Utility routine to build a volume grid for one of the surface grids.
// /hypeVolume (output) : volume grid
// /edgeSurface (input) : start from this surface grid.
// /linesToMarch, distanceToMarch (input) : parameters for the hyperbolic grid generator.
//
// =======================================================================  
{

  if( hypeVolume==NULL )
  {
    hypeVolume=new HyperbolicMapping;
    hypeVolume->incrementReferenceCount();
    mapInfo.mappingList.addElement(*hypeVolume);
    hypeVolume->decrementReferenceCount();
  }
  hypeVolume->setSurface( edgeSurface,false ); // false means this is a volume grid
  
  // set hyperbolic marching parameters
  IntegerArray ipar(5);
  RealArray rpar(5);

  ipar(0)=0; // region number
  ipar(1)=linesToMarch;
  hypeVolume->setParameters(HyperbolicMapping::linesInTheNormalDirection,ipar,rpar);

  ipar(0)=0; // region number
  rpar(0)=distanceToMarch;
  hypeVolume->setParameters(HyperbolicMapping::distanceToMarch,ipar,rpar);

  // hypeVolume->setParameters(HyperbolicMapping::growInTheReverseDirection,ipar,rpar);

  
  hypeVolume->generateNew();
  
  return 0;
}

int OffsetShell::
buildOffsetSurface(MappingInformation & mapInfo )
{
  
  if( offsetType==translationOffset )
  {
    if( offsetSurface==NULL )
    {
      offsetSurface = new MatrixTransform(*referenceSurface);
      offsetSurface->incrementReferenceCount();
      mapInfo.mappingList.addElement(*offsetSurface);
      offsetSurface->decrementReferenceCount();
    }

    MatrixTransform & offset = *((MatrixTransform*)offsetSurface);
    offset.reset();
    offset.shift( shift[0],shift[1],shift[2] );
  }
  else
  {
    if( offsetSurface==NULL )
    {
      offsetSurface = new NormalMapping(*referenceSurface,offsetDistance);
      offsetSurface->incrementReferenceCount();
      mapInfo.mappingList.addElement(*offsetSurface);
      offsetSurface->decrementReferenceCount();
    }
    NormalMapping & offset = *((NormalMapping*)offsetSurface);
    offset.setDomainDimension(2);   // we want a surface grid.
    offset.setNormalDistance(offsetDistance);

  }
  offsetSurface->setName(Mapping::mappingName,referenceSurface->getName(Mapping::mappingName)+"-offset");

  return 0;
}


void
buildCorner( int & m, int & numberOfCornerPoints, realArray & r, 
             real rc0, real rc1, real a, real b, real theta0, real theta1 )
// ======================================================================================
// /Description:
// utility routine to build an ellipse in a corner
// ======================================================================================
{
  for( int j=1; j< numberOfCornerPoints; j++ )
  {
    real theta = twoPi*( theta0+ (theta1-theta0)*j/numberOfCornerPoints );
    r(m,0)= rc0 + a*cos(theta);
    r(m,1)= rc1 + b*sin(theta);
    m++;
  }
  
}


int OffsetShell::
buildOffsetMappings( GenericGraphicsInterface & gi, 
                     GraphicsParameters & parameters, MappingInformation & mapInfo )
// =============================================================================================
/// \details 
///     Given a reference surface, build an offset surface, and then an edge surface to join two.
///   Build volume grids for the reference surface, offset surface and edge surface.
/// \param referenceSurface (input) : 
/// \param offsetSurface (output) :
/// \param edgeSurface, edgeVolume (output):
/// \param referenceVolume, offsetVolume (output) :
// ===============================================================================================
{
  bool adjustForAspectRatio=true;

  buildOffsetSurface(mapInfo );
  
  real offsetShift = offsetType==translationOffset ? SQRT( SQR(shift[0])+SQR(shift[1])+SQR(shift[2]) ) :
                     offsetDistance;

  realArray r4(4,1),rc(4,1);
  r4(0,0)=.125, r4(1,0)=.375, r4(2,0)=.625, r4(3,0)=.875;


  //
  //           --------------------------- ^
  //           |                         | | cornerOffset*rOffset
  //           |    /---------------\    | |
  //           |  /                  \   | v
  //           |  |                   |  |
  //           |  |                   |  |
  //           |  |                   |  |
  //           |  |                -->|  |<-- rOffset
  //           |  |                   |  |
  //           |  |                   |  |
  //           |  |                   |  |
  //           |  |                   |  |
  //           |  \------------------/   | 
  //           |                         |
  //           |-------------------------

  


  int debug=0;
  Range all,Rx=3;

  real aspectRatio[4]={1.,1.,1.,1.}; //
  if( adjustForAspectRatio )
  {
    // estimate the aspect ratio at the 4 corners.
    realArray r(4,2), x, xr(4,3,2);
    r(0,0)=0., r(0,1)=0.;
    r(1,0)=1., r(1,1)=0.;
    r(2,0)=1., r(2,1)=1.;
    r(3,0)=0., r(3,1)=1.;
    
    referenceSurface->map(r,x,xr);
    xr(all,Rx,0)*=1./max(1,referenceSurface->getGridDimensions(0)-1);
    xr(all,Rx,1)*=1./max(1,referenceSurface->getGridDimensions(1)-1);
    
    ::display(xr,"scaled xr");
    for( int m=0; m<4; m++ )
    {
      aspectRatio[m]=SQRT( sum(SQR(xr(m,Rx,0))) / max(REAL_MIN*10.,sum(SQR(xr(m,Rx,1)))) );
      printf(" corner m=%i : aspect ratio = %e\n",m,aspectRatio[m]);

    }
  }
  
  real maxFactor=4.75;

  real upperRightScaleFactor0 = aspectRatio[2] <= 1. ? 1./aspectRatio[2] : 1.;
  real upperRightScaleFactor1 = upperRightScaleFactor0*aspectRatio[2];
  if( upperRightScaleFactor0>maxFactor )
  {
     
    upperRightScaleFactor1=upperRightScaleFactor0/maxFactor;
    upperRightScaleFactor0=maxFactor;
    r4(0,0)=.25;
  }
  
  real upperLeftScaleFactor0 = aspectRatio[3] <= 1. ? 1./aspectRatio[3] : 1.;
  real upperLeftScaleFactor1 = upperLeftScaleFactor0*aspectRatio[3];
  if( upperLeftScaleFactor0>maxFactor )
  {
     
    upperLeftScaleFactor1=upperLeftScaleFactor0/maxFactor;
    upperLeftScaleFactor0=maxFactor;
    r4(1,0)=.25;
     
  }
  
  real lowerRightScaleFactor0 = aspectRatio[0] <= 1. ? 1./aspectRatio[0] : 1.;
  real lowerRightScaleFactor1 = lowerRightScaleFactor0*aspectRatio[0];
  
  real lowerLeftScaleFactor0 = aspectRatio[1] <= 1. ? 1./aspectRatio[1] : 1.;
  real lowerLeftScaleFactor1 = lowerLeftScaleFactor0*aspectRatio[1];

  // Build a curve in parameter space that follows the boundary of the unit square.

  int nr=9;    // number of points on a side.
  int numberOfCornerPoints=5;
  int numberOfPoints = (nr+numberOfCornerPoints-1-2)*4+1;
  
  Range I = numberOfPoints;
  
  realArray r(I,2);
  r=0.;
  
  real ra=1.-rOffset[1][0], sa=.5;
  real rb=ra,               sb=1.-rOffset[1][1]*cornerOffset*upperRightScaleFactor1;
  
  int n=(nr-1)/2;
  Range J = n;
  real ds = (sb-sa)/(n-1);
  r(J,0)=ra;
  r(J,1).seqAdd(sa,ds);
  

  // build a rounded corner : center = (rc0,rc1) (a,b) = half axes lengths.
  int m=n;
  //  r(m,0)=1.-rOffset[1][0]*cornerShift*upperRightScaleFactor0;  // upper right corner point
  //  r(m,1)=1.-rOffset[1][1]*cornerShift*upperRightScaleFactor1;
  real rc0=1.-rOffset[1][0]*cornerOffset*upperRightScaleFactor0;
  real rc1=sb;
  real a = rb-rc0, b=1.-rOffset[1][1]-sb;
  real theta0=0., theta1=.25;
  buildCorner( m, numberOfCornerPoints, r, rc0,rc1, a,b, theta0,theta1 );

  int na=m, nb=na+2*n-2;
  ra=1.-rOffset[1][0]*cornerOffset*upperRightScaleFactor0, rb=rOffset[0][0]*cornerOffset*upperLeftScaleFactor0;
  sa=1.-rOffset[1][1];   sb=sa;
  real dr = (rb-ra)/(nb-na);
  J=Range(na,nb);
  r(J,0).seqAdd(ra,dr);
  r(J,1)=sa;
  
  m=nb+1;
//  r(m,0)=   rOffset[0][0]*cornerShift*upperLeftScaleFactor0;      // upper left corner point
//  r(m,1)=1.-rOffset[1][1]*cornerShift*upperLeftScaleFactor1;
  rc0=rb;
  rc1=1.-rOffset[1][1]*cornerOffset*upperLeftScaleFactor1;
  a = rc0-rOffset[0][0], b=sb-rc1;
  theta0=.25, theta1=.5;
  buildCorner( m, numberOfCornerPoints, r, rc0,rc1, a,b, theta0,theta1 );

  na=m, nb=na+2*n-2;
  ra=rOffset[0][0]; rb=ra;
  sa=1.-rOffset[1][1]*cornerOffset*upperLeftScaleFactor1, sb=rOffset[0][1]*cornerOffset*lowerLeftScaleFactor1;
  ds = (sb-sa)/(nb-na);
  J=Range(na,nb);
  r(J,0)=ra;
  r(J,1).seqAdd(sa,ds);

  m=nb+1;
//  r(m,0)=rOffset[0][0]*cornerShift*lowerLeftScaleFactor0;      // lower left corner point
//  r(m,1)=rOffset[0][1]*cornerShift*lowerLeftScaleFactor1;
  rc0=rOffset[0][0]*cornerOffset*lowerLeftScaleFactor0;
  rc1=sb;
  a = rc0-rb, b=rc1-rOffset[0][1];
  theta0=.5, theta1=.75;
  buildCorner( m, numberOfCornerPoints, r, rc0,rc1, a,b, theta0,theta1 );

  na=m, nb=na+2*n-2;
  ra=rOffset[0][0]*cornerOffset*lowerLeftScaleFactor0, rb=1.-rOffset[1][0]*cornerOffset*lowerRightScaleFactor0;
  sa=rOffset[0][1];  sb=sa;
  dr = (rb-ra)/(nb-na);
  J=Range(na,nb);
  r(J,0).seqAdd(ra,dr);
  r(J,1)=sa;

  m=nb+1;
//  r(m,0)=1.-rOffset[1][0]*cornerShift*lowerRightScaleFactor0;      // lower right corner point
//  r(m,1)=   rOffset[0][1]*cornerShift*lowerRightScaleFactor1;
  rc0=rb;
  rc1=rOffset[0][1]*cornerOffset*lowerRightScaleFactor1;
  a = 1.-rOffset[1][0]-rc0, b=rc1-sb;
  theta0=.75, theta1=1.;
  buildCorner( m, numberOfCornerPoints, r, rc0,rc1, a,b, theta0,theta1 );

    
  na=m, nb=na+n-1;
  ra=1.-rOffset[1][0];
  sa=rOffset[0][1]*cornerOffset*lowerRightScaleFactor1, sb=.5;
  ds = (sb-sa)/(nb-na);
  J=Range(na,nb);
  r(J,0)=ra;
  r(J,1).seqAdd(sa,ds);
  
  SplineMapping spline(2);
  
  spline.setShapePreserving( true );
  spline.setIsPeriodic(axis1,Mapping::functionPeriodic);

  spline.setPoints( r(I,0), r(I,1) );
  spline.setGridDimensions(0,numberOfPoints*5);
  
  if( debug & 2 )
    spline.update(mapInfo);
  
  // Stretch the spline near corners
  StretchTransform stretchedSpline;
  stretchedSpline.setMapping( spline );
  StretchMapping & stretch = stretchedSpline.getStretchedSquare().stretchFunction(0);
  
  stretch.setNumberOfLayers( 4 );
  for( m=0; m<4; m++ )
  {
    real a=.1, b=50., c=r4(m,0); // .125+m/real(4);
    stretch.setLayerParameters( m,a,b,c );
  }
  if( debug & 2 )
    stretch.update(mapInfo);


  stretchedSpline.reinitialize();  // tell the StretchTransform that the stretching has changed.
  
  if( debug & 2 )
    stretchedSpline.update(mapInfo);
  
  // Now build the spline on the reference surface

  Mapping & rCurve = stretchedSpline;

  // evaluate the spline on a finer grid.
  int nFine=numberOfPoints*5;
  I=nFine;
  realArray r1(I,1), r2(I,2);
  dr=1./(nFine-1);
  r1.seqAdd(0.,dr);
  rCurve.map(r1,r2);  

  // get points on the reference surface
  realArray x(I,3);
  referenceSurface->map(r2,x);
  
  // build a spline so we can get the tangent
  SplineMapping xSpline0(3);
//  real arcLengthWeight=1., curvatureWeight=.2;
//  xSpline0.parameterize( arcLengthWeight, curvatureWeight );
//  xSpline.setParameterizationType(SplineMapping::index);
  xSpline0.setIsPeriodic(axis1,Mapping::functionPeriodic);

  // printf("build xSpline..\n");
  // display(x,"x","%9.3e ");
  
  x(nFine-1,Rx)=x(0,Rx);  // enforce periodicity
  xSpline0.setPoints( x(I,0), x(I,1), x(I,2) );
  xSpline0.setGridDimensions(0,nFine);

  if( debug & 2 )
   xSpline0.update(mapInfo);

  // Since the xSpline is reparameterized we must get this parameterization so we can
  // re-evaluate the ref surface at the same points (to get the normal).
  const realArray & s = xSpline0.getParameterization();
  SplineMapping sSpline;
//sSpline.setShapePreserving(true);
  sSpline.setPoints(s);

  // **** Stretch the xSpline at corners

  // Stretch the spline near corners
  StretchTransform xSplineStretched;
  xSplineStretched.setMapping( xSpline0 );
  StretchMapping & xStretch = xSplineStretched.getStretchedSquare().stretchFunction(0);
  
  xStretch.setNumberOfLayers( 4 );
  // find where the corners have shifted to on the xSpline:
  sSpline.map(r4,rc);  

  for( m=0; m<4; m++ )
  {
    real a=.1, b=cornerStretchExponent[m], c=rc(m,0); // .125+m/real(4);
    xStretch.setLayerParameters( m,a,b,c );
  }
  if( debug & 2 )
    xStretch.update(mapInfo);

  Mapping & xSpline = xSplineStretched;

  // Compute the tangent to the curve and normal to the surface

  
  int nGrid=numberOfEdgeLines[0]; // numberOfPoints*2; // here is the actual number of points we use
  I=nGrid;
  r1.redim(I,1);
  dr=1./(nGrid-1);
  r1.seqAdd(0.,dr);

  realArray rs1(I,1),rs(I,1);
//  rs1=-1.;
  xStretch.map(r1,rs1);
  rs=-1.;
  sSpline.inverseMap(rs1,rs);   // rs : evaluate rCurve at these new parameter points to correspond to xSpline points.
  
  r2.redim(I,2);
  rCurve.map(rs,r2);
  
  realArray tangent(I,3,1);
  x.redim(I,3);

  xSpline.map(r1,x,tangent); 
  
  realArray norm;
  norm= SQRT( SQR(tangent(I,0))+SQR(tangent(I,1))+SQR(tangent(I,2)) );
  norm=1./max(REAL_EPSILON*100.,norm);
  int axis;
  for( axis=0; axis<3; axis++ )
    tangent(I,axis)*=norm;
  

  realArray xr(I,3,2);
  referenceSurface->map(r2,x,xr);
  
  realArray normal(I,3);

  normal(I,0)=xr(I,1,0)*xr(I,2,1)-xr(I,2,0)*xr(I,1,1);
  normal(I,1)=xr(I,2,0)*xr(I,0,1)-xr(I,0,0)*xr(I,2,1);
  normal(I,2)=xr(I,0,0)*xr(I,1,1)-xr(I,1,0)*xr(I,0,1);
  
  norm= SQRT( SQR(normal(I,0))+SQR(normal(I,1))+SQR(normal(I,2)) );
  norm=1./max(REAL_EPSILON*100.,norm);
  for( axis=0; axis<3; axis++ )
    normal(I,axis)*=norm;
  
  // -- build the direction vector orthogonal to the normal and tangent  d= tangent X normal 
  realArray d(I,3);
  
  d(I,0)=tangent(I,1)*normal(I,2)-tangent(I,2)*normal(I,1);
  d(I,1)=tangent(I,2)*normal(I,0)-tangent(I,0)*normal(I,2);
  d(I,2)=tangent(I,0)*normal(I,1)-tangent(I,1)*normal(I,0);
  
  norm= SQRT( SQR(d(I,0))+SQR(d(I,1))+SQR(d(I,2)) );
  norm=1./max(REAL_EPSILON*100.,norm);
  for( axis=0; axis<3; axis++ )
    d(I,axis)*=norm;
  

  if( debug & 2 )
  {
    display(tangent,"tangent","%8.2e " );
    display(normal,"normal","%8.2e ");
  }
  
  if( edgeSurface==NULL )
  {
    edgeSurface = new DataPointMapping;

    edgeSurface->incrementReferenceCount();
    mapInfo.mappingList.addElement(*edgeSurface);
    edgeSurface->decrementReferenceCount();
  }
  
  edgeSurface->setName(Mapping::mappingName,referenceSurface->getName(Mapping::mappingName)+"-edgeSurface");


  int ns=numberOfEdgeLines[1];;  // number of points along the 'round' of the edge
  J=ns;
  int rangeDimension=3;
  realArray edge(I,J,rangeDimension);
  
  x.reshape(I,1,Rx);
  edge(I,0,Rx)=x(I,0,Rx);

  //      x--x--x
  //             
  //
  //      x--x--x

  real roundDistance=offsetShift*.5;
  real roundDiameter=roundDistance*twoPi*.5;
  const int numberOfPointsOnArc=ns-numberOfMatchingPoints*2;
  real delta = roundDiameter/max(1,numberOfPointsOnArc-1);  // arclength of each point on the circular round

  
  d.reshape(I,1,Rx);
  
  edge(I,0,Rx)=x(I,0,Rx);
  int j;
  n=1;
  for( j=0; j<numberOfMatchingPoints; j++ )
  {
    edge(I,n,Rx)=edge(I,n-1,Rx)+d(I,0,Rx)*delta;   // move in the direction of d on the ref surf.
    n++;
  }

  // Make a circular end
  for( j=1; j<numberOfPointsOnArc; j++ )
  {
    real theta = j*.5*twoPi/(numberOfPointsOnArc-1);
    real dx=.5-.5*cos(theta);
    real dy=sin(theta);
    
    if( offsetType==translationOffset )
    {
      for( axis=0; axis<3; axis++ )
      {
	edge(I,n+j-1,axis)=edge(I,n-1,axis)+shift[axis]*dx + d(I,0,axis)*roundDistance*dy;
      }
    }
    else
    {
      for( axis=0; axis<3; axis++ )
      {
	edge(I,n+j-1,axis)=edge(I,n-1,axis)+normal(I,axis)*(dx*offsetDistance) + d(I,0,axis)*roundDistance*dy;
      }
    }
    
  }
  n+=numberOfPointsOnArc-1;
  for( j=0; j<numberOfMatchingPoints; j++ )
  {
    edge(I,n,Rx)=edge(I,n-1,Rx)-d(I,0,Rx)*delta;  // move backward in the direction d on the offset surface.
    n++;
  }
  assert( n==ns );

  int domainDimension=2;
  edgeSurface->setIsPeriodic(axis1,Mapping::functionPeriodic);
  edgeSurface->setDataPoints(edge,2,domainDimension);

  if( false )
  {
    edgeSurface->update(mapInfo);
  }


  return 0;
}

int OffsetShell::
generateVolumeGrids( GenericGraphicsInterface & gi, 
                     GraphicsParameters & parameters, MappingInformation & mapInfo )
// =============================================================================================
/// \details 
///     Build the volume grids.
// ===============================================================================================
{
  int shareFlag=2;

  int linesToMarch=numberOfEdgeLines[2];
  real distanceToMarch=normalDistance;
  printf("**generateVolumeGrids: linesToMarch=%i, distanceToMarch=%e\n",linesToMarch,distanceToMarch);

  buildVolumeGrid( edgeVolume,*edgeSurface,linesToMarch,distanceToMarch,mapInfo );
  edgeVolume->setShare(Start,axis3,shareFlag);

  printf("Set the share flag on side (0,2) of the edgeVolume, referenceVolume and offsetVolume to %i\n",shareFlag);
  

  if( false )
  {
    edgeVolume->update(mapInfo);
  }

  buildVolumeGrid( referenceVolume,*referenceSurface,linesToMarch,-distanceToMarch,mapInfo );
  referenceVolume->setShare(Start,axis3,shareFlag);

  buildVolumeGrid( offsetVolume,*offsetSurface,linesToMarch,distanceToMarch,mapInfo );
  offsetVolume->setShare(Start,axis3,shareFlag);

  return 0;
}


int OffsetShell::
createOffsetMappings( MappingInformation & mapInfo )
//===============================================================================================
/// \details 
///      Interactively build grids for a thin shell.
/// 
//===============================================================================================
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!OffsetShell",
      "choose a mapping",
      " ",
      "plot",
      "help",
      "exit", 
      "" 
     };

  GUIState gui;
  gui.setWindowTitle("Offset Shell");
  gui.setExitCommand("exit", "exit");
  DialogData & dialog = (DialogData &)gui;

  dialog.setOptionMenuColumns(1);

  bool referenceSurfaceChosen=referenceSurface!=NULL;
  int optionMenuChoice=0;
  
  const int num=mapInfo.mappingList.getLength();
  aString *label = new aString[num+2];
  aString *cmd = new aString [num+2];
  int j=0;
  for( int i=num-1; i>=0; i-- )
  {
    MappingRC & map = mapInfo.mappingList[i];
    if( map.getDomainDimension()==2 && map.getRangeDimension()==3 )
    {
      label[j]=map.getName(Mapping::mappingName);
      cmd[j]="Reference Surface:"+label[j];
      if( referenceSurface==NULL )
      {
	referenceSurface=mapInfo.mappingList[i].mapPointer;
	referenceSurface->incrementReferenceCount();
      }
      if( referenceSurfaceChosen && 
          referenceSurface->getName(Mapping::mappingName)==map.getName(Mapping::mappingName) )
      {
        // local existing reference surface in the list so we can set the default option menu.
	optionMenuChoice=j;
      }
      j++;
      
    }
  }
  label[j]=""; cmd[j]="";   // null string terminates the menu

  // addPrefix(label,prefix,cmd,maxCommands);
  dialog.addOptionMenu("Reference Surface:", cmd,label,optionMenuChoice);

  delete [] cmd;
  delete [] label;

  const int numLabels=3;
  aString opLabel[numLabels] = {"translation offset",
				"normal offset",
				""}; //
//  aString opCmd[numLabels];
//  GUIState::addPrefix(opLabel,"initial curve:",opCmd,numLabels);

  dialog.addOptionMenu("offset curve from:",opLabel,opLabel,(int)offsetType);


  aString pbLabels[] = {"generate volume grids",
                        "generate edge surface",
			""};
  // addPrefix(pbLabels,prefix,cmd,maxCommands);
  int numRows=2;
  dialog.setPushButtons( pbLabels, pbLabels, numRows ); 


  const int numberOfTextStrings=10;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  real distanceToMarch=.1;
  int nt=0;
  textLabels[nt] = "offset vector"; 
  sPrintF(textStrings[nt], "%g, %g, %g", shift[0],shift[1],shift[2]); nt++; 

  textLabels[nt] = "number of matching points"; 
  sPrintF(textStrings[nt], "%i",numberOfMatchingPoints); nt++; 

  textLabels[nt] = "relative offset from edge"; 
  sPrintF(textStrings[nt], 
     "%g, %g, %g, %g (left,right,bottom,top)",rOffset[0][0],rOffset[1][0],rOffset[0][1],rOffset[1][1]); nt++; 

  textLabels[nt] = "normal distance"; 
  sPrintF(textStrings[nt], "%g",normalDistance); nt++; 

  textLabels[nt] = "lines in normal direction"; 
  sPrintF(textStrings[nt], "%i",numberOfEdgeLines[2]); nt++; 

  textLabels[nt] = "lines on edge surface"; 
  sPrintF(textStrings[nt], "%i, %i",numberOfEdgeLines[0],numberOfEdgeLines[1]); nt++; 

  textLabels[nt] = "offset distance"; 
  sPrintF(textStrings[nt], "%g",offsetDistance); nt++; 

  textLabels[nt] = "corner stretch exponent"; 
  sPrintF(textStrings[nt],"%g, %g, %g, %g ",cornerStretchExponent[0],cornerStretchExponent[1],
          cornerStretchExponent[2],cornerStretchExponent[3]); nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );

  // addPrefix(textLabels,prefix,cmd,maxCommands);
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  gui.buildPopup(menu);



  aString answer,line,answer2; 

  bool plotObject=true;


  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,true);

  bool volumeGridsAreValid=false;
  int axis;

  bool executeCommand=false;
  if( !executeCommand  )
  {
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt("offset>"); // set the default prompt
  }

  int len;
  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getAnswer(answer,"");
 

    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    if( len=answer.matches("Reference Surface:") )
    {
      aString name=answer(len,answer.length()-1);
      const int num=mapInfo.mappingList.getLength();
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( map.getDomainDimension()==2 && map.getRangeDimension()==3 )
	{
	  if( name==map.getName(Mapping::mappingName) )
	  {
	    if( referenceSurface!=0 && referenceSurface->decrementReferenceCount()==0 ) 
	      delete referenceSurface;
	    referenceSurface=mapInfo.mappingList[i].mapPointer;
	    referenceSurface->incrementReferenceCount();

   	    buildOffsetMappings( gi, parameters, mapInfo );

            break;
	  }
	}
      }
    }
    else if( answer=="choose a mapping" )
    { 
      // Make a menu with the Mapping names
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+1];
      for( int i=0; i<num; i++ )
        menu2[i]=mapInfo.mappingList[i].getName(Mapping::mappingName);
      menu2[num]="";   // null string terminates the menu
      for( ;; )
      {
	int mapNumber = gi.getMenuItem(menu2,line);
        if( mapNumber<0 )
	{
	  printf("UnstructuredMapping::ERROR:unknown mapping to turn into an offset!\n");
	  gi.stopReadingCommandFile();
	}
	else
	{
	  referenceSurface  = &mapInfo.mappingList[mapNumber].getMapping();

	  buildOffsetMappings( gi, parameters, mapInfo );

	  break;
	}

      }
      delete [] menu2;

    }
    else if( answer=="generate edge surface" )
    {
      if( referenceSurface!=NULL )
	  buildOffsetMappings( gi, parameters, mapInfo );
      else
      {
        printf("ERROR: you must define a reference surface first\n");
      }
      
    }
    else if( answer=="generate volume grids" )
    {
      generateVolumeGrids( gi, parameters, mapInfo );
      volumeGridsAreValid=true;
    }
    else if( len=answer.matches("offset vector") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&shift[0],&shift[1],&shift[2]);
      dialog.setTextLabel(0,sPrintF(line,"%g, %g, %g",shift[0],shift[1],shift[2]));
      volumeGridsAreValid=false;
    }
    else if( len=answer.matches("number of matching points") )
    {
      sScanF(answer(len,answer.length()-1),"%i",&numberOfMatchingPoints);
      printf("number of matching points=%i\n",numberOfMatchingPoints);
      numberOfMatchingPoints=max(0,numberOfMatchingPoints);
      dialog.setTextLabel(1,sPrintF(line,"%i ",numberOfMatchingPoints));
      volumeGridsAreValid=false;
    }
    else if( len=answer.matches("relative offset from edge") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e %e",&rOffset[0][0],&rOffset[1][0],
                &rOffset[0][1],&rOffset[1][1]);
      dialog.setTextLabel(2,sPrintF(line,"%g, %g, %g, %g (left,right,bottom,top)",
                 rOffset[0][0],rOffset[1][0],rOffset[0][1],rOffset[1][1]));

      volumeGridsAreValid=false;
    }
    else if( len=answer.matches("normal distance") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&normalDistance);
      dialog.setTextLabel(3,sPrintF(line,"%g",normalDistance));
      volumeGridsAreValid=false;
    }
    else if( len=answer.matches("lines in normal direction") )
    {
      sScanF(answer(len,answer.length()-1),"%i",&numberOfEdgeLines[2]);
      dialog.setTextLabel(4,sPrintF(line,"%i ",numberOfEdgeLines[2]));
      volumeGridsAreValid=false;
    }
    else if( len=answer.matches("lines on edge surface") )
    {
      sScanF(answer(len,answer.length()-1),"%i %i",&numberOfEdgeLines[0],&numberOfEdgeLines[1]);
      dialog.setTextLabel(5,sPrintF(line,"%i, %i",numberOfEdgeLines[0],numberOfEdgeLines[1]));
      volumeGridsAreValid=false;
    }
    else if( len=answer.matches("offset distance") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&offsetDistance);
      dialog.setTextLabel(6,sPrintF(line,"%g",offsetDistance));
      volumeGridsAreValid=false;
    }
    else if( len=answer.matches("corner stretch exponent") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e %e",&cornerStretchExponent[0],&cornerStretchExponent[1],
	     &cornerStretchExponent[2],&cornerStretchExponent[3]);
      dialog.setTextLabel(7,sPrintF(line,"%g, %g, %g, %g ",cornerStretchExponent[0],cornerStretchExponent[1],
          cornerStretchExponent[2],cornerStretchExponent[3]));

      volumeGridsAreValid=false;
    }


    else if( answer=="translation offset" || answer=="normal offset" )
    {
      offsetType=answer=="translation offset" ? translationOffset : normalOffset;
      dialog.getOptionMenu(1).setCurrentChoice(offsetType);
      // remove the offset surface if it is of the wrong type.
      if( offsetSurface!=NULL )
      {
         if( (offsetType==normalOffset && offsetSurface->getClassName()!="NormalMapping" ) ||
             (offsetType==translationOffset && offsetSurface->getClassName()!="MatrixTransform" ) )
	 {
	   if( offsetSurface->decrementReferenceCount()==0 )
	     delete offsetSurface;
	   offsetSurface=NULL;
	 }
      }
      
    }
    else if( answer=="plotObject" )
    {
    }
    else
    {
      printf("Unknown response [%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

    if( plotObject )
    {
      gi.erase();
      if( volumeGridsAreValid && referenceVolume!=NULL )
      {
        parameters.set(GI_MAPPING_COLOUR,"blue");
	PlotIt::plot( gi,*referenceVolume,parameters );
      }
      else if( referenceSurface!=NULL )
      {
        parameters.set(GI_MAPPING_COLOUR,"blue");
	PlotIt::plot( gi,*referenceSurface,parameters );
      }
      if( volumeGridsAreValid && offsetVolume!=NULL )
      {
        parameters.set(GI_MAPPING_COLOUR,"green");
	PlotIt::plot( gi,*offsetVolume,parameters );
      }
      else if( offsetSurface!=NULL )
      {
        parameters.set(GI_MAPPING_COLOUR,"green");
	PlotIt::plot( gi,*offsetSurface,parameters );
      }
      if( volumeGridsAreValid && edgeVolume!=NULL )
      {
        parameters.set(GI_MAPPING_COLOUR,"red");
	PlotIt::plot( gi,*edgeVolume,parameters );
      }
      else if( edgeSurface!=NULL )
      {
        parameters.set(GI_SURFACE_OFFSET,(real)10.);  // offset the surface so it does show through the others.
        parameters.set(GI_MAPPING_COLOUR,"red");
	PlotIt::plot( gi,*edgeSurface,parameters );
        parameters.set(GI_SURFACE_OFFSET,(real)3.);  // reset
      }
      parameters.set(GI_MAPPING_COLOUR,"red");

    }
  }
  
  if( !executeCommand  )
  {
    gi.erase();
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }
  
  return 0;
}
