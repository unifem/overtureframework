#include "RotatedBox.h"

RotatedBox::
RotatedBox(int numberOfDimensions_ )
// =========================================================================
// /Description:
// The RotatedBox class is used to define a refinement grid for AMR
// Given a set of points in space (tagged error points) a rotated
// box with be determined that contains all the points.
// =========================================================================
{
  numberOfDimensions=numberOfDimensions_;
}

RotatedBox::
RotatedBox( int numberOfDimensions_, realArray & x )
{
  numberOfDimensions=numberOfDimensions_;
  setPoints(x);
}


RotatedBox::
~RotatedBox()
{
}

int RotatedBox::
display() const 
{
  printf(" box: centre=(%8.2e,%8.2e) axesLength=(%8.2e,%8.2e) e0=(%8.2e,%8.2e), e1=(%8.2e,%8.2e)\n",
	 centre[0],centre[1],halfAxesLength[0],halfAxesLength[1],
         axisVector[0][0],axisVector[1][0],axisVector[0][1],axisVector[1][1]);
  return 0;
}


bool RotatedBox::
intersects( RotatedBox & box, real distance /* =0. */ ) const
// =================================================================================
// /Description:
//   Does this box intersect another
// ================================================================================
{

  real centreSeparation= SQRT( SQR( box.centre[0]-centre[0] )+ SQR(box.centre[1]-centre[1]) );
  
  real dist1 = SQRT( SQR(box.halfAxesLength[0]) +SQR(box.halfAxesLength[1]) );
  real dist2 = SQRT( SQR(    halfAxesLength[0]) +SQR(    halfAxesLength[1]) );
  if( centreSeparation > (dist1+dist2)+distance )
  {
    return false;
  }
  else
  {
    return true;
  }
}


int RotatedBox::
numberOfPoints() const
{
  return 20;
}


real RotatedBox::
getEfficiency() const
{
  real xArea=sum(xa(nullRange,xa.getBound(1)));
  real area=fabs(4.*halfAxesLength[0]*halfAxesLength[1]);

  // printf(" getEfficiency: xArea=%e, area=%e\n",xArea,area);
  
  if( area>0. )
    return xArea/area;
  else
    return 1.;
//  return 0.;
}

int RotatedBox::
setPoints( realArray & x )
{
  xa.reference(x);
  fitBox();
  return 0;
}


int RotatedBox::
fitBox()
// ====================================================================================
// /Description:
//    Determine a rotated box that contains all the points in xa.
// ===================================================================================
{
  real mean[3]={0.,0.,0.};  // center of the ellipse

  real xc[3]={0.,0.,0.};  
  // real xMin[3]={0.,0.,0.};  
  // real xMax[3]={0.,0.,0.};  

  realArray m(numberOfDimensions,numberOfDimensions); // matrix of second moments

  Range R=xa.dimension(0);
  
  int axis;
  for( axis=0; axis<numberOfDimensions; axis++ )
  {
    mean[axis]=sum(xa(R,axis))/real(R.getLength());
//     xMin[axis]=min(xa(R,axis));
//     xMax[axis]=max(xa(R,axis));
    
//     xc[axis]= .5*(xMin[axis]+xMax[axis]); // mean[axis]-.5*( dMax+dMin );  // could take .5( min()+max() )

    xc[axis]=mean[axis];

  }
  
  for( axis=0; axis<numberOfDimensions; axis++ )
  {
    for( int dir=0; dir<=axis; dir++ )
    {
      m(axis,dir)=sum((xa(R,axis)-mean[axis])*(xa(R,dir)-mean[dir]));  // is subtracting mean necessary?
    }
  }

  // M = [ a c ]
  //     [ c b ]

  real a=m(0,0), b=m(1,1), c=m(1,0);
  
  real q = SQRT( .25*SQR(a-b)+c*c );
  
  real lambda1 = (a+b)*.5+q;
  real lambda2 = (a+b)*.5-q;
  
  RealArray ev(3,3);  // eigenvectors
  ev=0.;
  
  ev(0,0)=(b-lambda1);
  ev(1,0)=-c;
  real norm = SQRT( SQR(ev(0,0))+SQR(ev(1,0)));
  ev(0,0)/=norm;  ev(1,0)/=norm;
  
  ev(0,1)=c;
  ev(1,1)=-(a-lambda2);
  norm = SQRT( SQR(ev(0,1))+SQR(ev(1,1)));
  ev(0,1)/=norm;  ev(1,1)/=norm;

  real ax[3]={0.,0.,0.,};  // half the length of the axes
  for( axis=0; axis<numberOfDimensions; axis++ )
  {
    realArray d;
    d=ev(0,axis)*(xa(R,0)-xc[0])+ev(1,axis)*(xa(R,1)-xc[1]);
    real dMax=max(d);
    real dMin=min(d);
    ax[axis] = (dMax-dMin)*.5;
    
    xc[0]+= .5*(dMax+dMin)*ev(0,axis); // adjust the centre to be in the middle.
    xc[1]+= .5*(dMax+dMin)*ev(1,axis);
    
  }
  for( axis=0; axis<numberOfDimensions; axis++ )
  {
    ev(0,axis)*=ax[axis]; // scale eigen vectors by the half axis length
    ev(1,axis)*=ax[axis];
  }

  // corners of the box:
  // box(0,axis)=mean[axis]-ax[0]*ev(0,axis)-ax[1]*ev(1,axis);
  // box(1,axis)=mean[axis]+ax[0]*ev(0,axis)+ax[1]*ev(1,axis);
  
  printf("+++RotatedBox: centre=(%8.2e,%8.2e) axesLength=(%8.2e,%8.2e) e0=(%8.2e,%8.2e), e1=(%8.2e,%8.2e)\n",
	 xc[0],xc[1],ax[0],ax[1],ev(0,0),ev(1,0),ev(0,1),ev(1,1));


  centre[0]=xc[0];
  centre[1]=xc[1];
  centre[2]=xc[2];

  axisVector[0][0]=ev(0,0);
  axisVector[1][0]=ev(1,0);
  axisVector[2][0]=ev(2,0);
  axisVector[0][1]=ev(0,1);
  axisVector[1][1]=ev(1,1);
  axisVector[2][1]=ev(2,1);
  axisVector[0][2]=ev(0,2);
  axisVector[1][2]=ev(1,2);
  axisVector[2][2]=ev(2,2);
  

  halfAxesLength[0]=ax[0];
  halfAxesLength[1]=ax[1];
  halfAxesLength[2]=ax[2];
  
  return 0;
}
