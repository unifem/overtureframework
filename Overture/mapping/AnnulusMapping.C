// This file automatically generated from AnnulusMapping.bC with bpp.
#include "AnnulusMapping.h"
#include "MappingInformation.h"
#include <float.h>


//\begin{>AnnulusMappingInclude.tex}{\subsection{Constructor}}
AnnulusMapping::
AnnulusMapping(const real innerRadius_ /* =.5 */, 
             	       const real outerRadius_ /* =1. */, 
             	       const real x0_          /* =0. */, 
             	       const real y0_          /* =0. */, 
             	       const real startAngle_  /* =0. */,
             	       const real endAngle_    /* =1. */,
                              const real aOverB_ /* =1. */ )
: Mapping(2,2,parameterSpace,cartesianSpace)   
//===========================================================================
// /Purpose: Create an annulus with a circular or elliptical boundary.
// 
// The annulus is defined by 
// \begin{align*}  
//        x(r,s) &= x0 + {\rm aOverB}~ R(s)\cos(\theta(r)) \cr
//        y(r,s) &= y0 +        R(s)\sin(\theta(r))    \cr   
//        R(s) &= {\rm innerRadius} + s ({\rm outerRadius}-{\rm innerRadius})  \cr
//        \theta(r) &= 2 \pi [{\rm startAngle} + r ({\rm endAngle}-{\rm startEngle})] 
// \end{align*} 
// /innerRadius,outerRadius (input):  inner and outer radii.
// /x0,y0 (input): centre for the annulus.
// /startAngle, endAngle (input): The initial and final "angle" (in the range [0,1]).
// /aOverB (input): The ratio of the length of the horizontal-radius ("a") to the vertical-radius ("b")
//  for an elliptical boundary. A value of aOverB=1 defines a circular boundary. 
// 
//\end{AnnulusMappingInclude.tex}
//===========================================================================
{
    AnnulusMapping::className="AnnulusMapping";
    setBasicInverseOption(canInvert);  // basicInverse is available
    inverseIsDistributed=false; // *wdh* 2015/05/25 

    setName( Mapping::mappingName,"Annulus");
    setName(Mapping::domainAxis1Name,"theta");
    setName(Mapping::domainAxis2Name,"radius");
    
    setGridDimensions( axis1,21 );  // gridlines for plotting and inverse
    setGridDimensions( axis2,7 ); 
    innerRadius=innerRadius_;
    outerRadius=outerRadius_;
    aOverB=aOverB_;

    signForJacobian=-1.;            // default Annulus is left handed.
    
    x0=x0_; y0=y0_;
    setAngleBounds(startAngle_,endAngle_);
    zLevel=0.;
    setBoundaryCondition( Start,axis2,1 );
    setBoundaryCondition(   End,axis2,2 );
    mappingHasChanged();

}


// Copy constructor is deep by default
AnnulusMapping::
AnnulusMapping( const AnnulusMapping & map, const CopyType copyType )
{
    AnnulusMapping::className="AnnulusMapping";
    if( copyType==DEEP )
    {
        *this=map;
    }
    else
    {
        cout << "AnnulusMapping:: sorry no shallow copy constructor, doing a deep! \n";
        *this=map;
    }
}

AnnulusMapping::
~AnnulusMapping()
{ if( debug & 4 )
        cout << " AnnulusMapping::Destructor called" << endl;
}

// ====================================================================
// RANGEDIM: 2,3
// OPTION: computeMap,computeMapDerivative,computeBoth
// ====================================================================


void AnnulusMapping::
map( const realArray & r_, realArray & x_, realArray & xr_, MappingParameters & params)
// =========================================================================================
// 
// =========================================================================================
{
    if( params.coordinateType != cartesian )
        cerr << "AnnulusMapping::map - coordinateType != cartesian " << endl;

    Index I = getIndex( r_,x_,xr_,base,bound,computeMap,computeMapDerivative );

    #ifndef USE_PPP
        const realSerialArray & r = r_;
        realSerialArray & x = x_;
        realSerialArray & xr = xr_;
    #else
        const realSerialArray & r = r_.getLocalArray();
        const realSerialArray & x = x_.getLocalArray();
        const realSerialArray & xr = xr_.getLocalArray();
        base =max(I.getBase(), r.getBase(0));
        bound=min(I.getBound(),r.getBound(0));
        I=Range(base,bound);
    #endif


    real scale=twoPi*(endAngle-startAngle);
    real rad=outerRadius-innerRadius;

    const real * rp = r.Array_Descriptor.Array_View_Pointer1;
    const int rDim0=r.getRawDataSize(0);
#undef R
#define R(i0,i1) rp[i0+rDim0*(i1)]
    real * xp = x.Array_Descriptor.Array_View_Pointer1;
    const int xDim0=x.getRawDataSize(0);
#undef X
#define X(i0,i1) xp[i0+xDim0*(i1)]
    real * xrp = xr.Array_Descriptor.Array_View_Pointer2;
    const int xrDim0=xr.getRawDataSize(0);
    const int xrDim1=xr.getRawDataSize(1);
#undef XR
#define XR(i0,i1,i2) xrp[i0+xrDim0*(i1+xrDim1*(i2))]

    if( computeMap && computeMapDerivative )
    {
        if( rangeDimension==2 )
      // compute(2,computeBoth);
            {
              real angle,radius,sina,cosa;
              for( int i=base; i<=bound; i++ )
              {
                  angle=scale*R(i,axis1)+twoPi*startAngle;
                  radius=rad*R(i,axis2)+innerRadius;
                  sina=sin(angle);
                  cosa=cos(angle);
         //          #If "computeBoth" == "computeMap" || "computeBoth" == "computeBoth"
                      X(i,axis1)=aOverB*radius*cosa+x0;
                      X(i,axis2)=       radius*sina+y0;
           //            #If "2" == "3"
           //          #If "computeBoth" == "computeMapDerivative" || "computeBoth" == "computeBoth"  
                      XR(i,axis1,axis1)=-aOverB*radius*scale*sina;
                      XR(i,axis2,axis1)=        radius*scale*cosa;
                      XR(i,axis1,axis2)=aOverB*rad*cosa;
                      XR(i,axis2,axis2)=       rad*sina;
           //            #If "2" == "3"
              }
            }
        else
      // compute(3,computeBoth);
            {
              real angle,radius,sina,cosa;
              for( int i=base; i<=bound; i++ )
              {
                  angle=scale*R(i,axis1)+twoPi*startAngle;
                  radius=rad*R(i,axis2)+innerRadius;
                  sina=sin(angle);
                  cosa=cos(angle);
         //          #If "computeBoth" == "computeMap" || "computeBoth" == "computeBoth"
                      X(i,axis1)=aOverB*radius*cosa+x0;
                      X(i,axis2)=       radius*sina+y0;
           //            #If "3" == "3"
                          X(i,axis3)=zLevel;
             //          #If "computeBoth" == "computeMapDerivative" || "computeBoth" == "computeBoth"  
                      XR(i,axis1,axis1)=-aOverB*radius*scale*sina;
                      XR(i,axis2,axis1)=        radius*scale*cosa;
                      XR(i,axis1,axis2)=aOverB*rad*cosa;
                      XR(i,axis2,axis2)=       rad*sina;
           //            #If "3" == "3"
                          XR(i,axis3,0)=0.;
                          XR(i,axis3,1)=0.;
              }
            }
    }
    else if( computeMap )
    {
        if( rangeDimension==2 )
      // compute(2,computeMap);
            {
              real angle,radius,sina,cosa;
              for( int i=base; i<=bound; i++ )
              {
                  angle=scale*R(i,axis1)+twoPi*startAngle;
                  radius=rad*R(i,axis2)+innerRadius;
                  sina=sin(angle);
                  cosa=cos(angle);
         //          #If "computeMap" == "computeMap" || "computeMap" == "computeBoth"
                      X(i,axis1)=aOverB*radius*cosa+x0;
                      X(i,axis2)=       radius*sina+y0;
           //            #If "2" == "3"
           //          #If "computeMap" == "computeMapDerivative" || "computeMap" == "computeBoth"  
              }
            }
        else
      // compute(3,computeMap);
            {
              real angle,radius,sina,cosa;
              for( int i=base; i<=bound; i++ )
              {
                  angle=scale*R(i,axis1)+twoPi*startAngle;
                  radius=rad*R(i,axis2)+innerRadius;
                  sina=sin(angle);
                  cosa=cos(angle);
         //          #If "computeMap" == "computeMap" || "computeMap" == "computeBoth"
                      X(i,axis1)=aOverB*radius*cosa+x0;
                      X(i,axis2)=       radius*sina+y0;
           //            #If "3" == "3"
                          X(i,axis3)=zLevel;
             //          #If "computeMap" == "computeMapDerivative" || "computeMap" == "computeBoth"  
              }
            }
    }
    else if( computeMapDerivative )
    {
        if( rangeDimension==2 )
      // compute(2,computeMapDerivative);
            {
              real angle,radius,sina,cosa;
              for( int i=base; i<=bound; i++ )
              {
                  angle=scale*R(i,axis1)+twoPi*startAngle;
                  radius=rad*R(i,axis2)+innerRadius;
                  sina=sin(angle);
                  cosa=cos(angle);
         //          #If "computeMapDerivative" == "computeMap" || "computeMapDerivative" == "computeBoth"
         //          #If "computeMapDerivative" == "computeMapDerivative" || "computeMapDerivative" == "computeBoth"  
                      XR(i,axis1,axis1)=-aOverB*radius*scale*sina;
                      XR(i,axis2,axis1)=        radius*scale*cosa;
                      XR(i,axis1,axis2)=aOverB*rad*cosa;
                      XR(i,axis2,axis2)=       rad*sina;
           //            #If "2" == "3"
              }
            }
        else
      // compute(3,computeMapDerivative);
            {
              real angle,radius,sina,cosa;
              for( int i=base; i<=bound; i++ )
              {
                  angle=scale*R(i,axis1)+twoPi*startAngle;
                  radius=rad*R(i,axis2)+innerRadius;
                  sina=sin(angle);
                  cosa=cos(angle);
         //          #If "computeMapDerivative" == "computeMap" || "computeMapDerivative" == "computeBoth"
         //          #If "computeMapDerivative" == "computeMapDerivative" || "computeMapDerivative" == "computeBoth"  
                      XR(i,axis1,axis1)=-aOverB*radius*scale*sina;
                      XR(i,axis2,axis1)=        radius*scale*cosa;
                      XR(i,axis1,axis2)=aOverB*rad*cosa;
                      XR(i,axis2,axis2)=       rad*sina;
           //            #If "3" == "3"
                          XR(i,axis3,0)=0.;
                          XR(i,axis3,1)=0.;
              }
            }
    }
#undef X
#undef X
#undef XR

    
}

void AnnulusMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params)
// =========================================================================================
//   new *serial-array version*
// =========================================================================================
{
    if( params.coordinateType != cartesian )
        cerr << "AnnulusMapping::map - coordinateType != cartesian " << endl;

    Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

    real scale=twoPi*(endAngle-startAngle);
    real rad=outerRadius-innerRadius;

    const real * rp = r.Array_Descriptor.Array_View_Pointer1;
    const int rDim0=r.getRawDataSize(0);
#undef R
#define R(i0,i1) rp[i0+rDim0*(i1)]
    real * xp = x.Array_Descriptor.Array_View_Pointer1;
    const int xDim0=x.getRawDataSize(0);
#undef X
#define X(i0,i1) xp[i0+xDim0*(i1)]
    real * xrp = xr.Array_Descriptor.Array_View_Pointer2;
    const int xrDim0=xr.getRawDataSize(0);
    const int xrDim1=xr.getRawDataSize(1);
#undef XR
#define XR(i0,i1,i2) xrp[i0+xrDim0*(i1+xrDim1*(i2))]

    if( computeMap && computeMapDerivative )
    {
        if( rangeDimension==2 )
      // compute(2,computeBoth);
            {
              real angle,radius,sina,cosa;
              for( int i=base; i<=bound; i++ )
              {
                  angle=scale*R(i,axis1)+twoPi*startAngle;
                  radius=rad*R(i,axis2)+innerRadius;
                  sina=sin(angle);
                  cosa=cos(angle);
         //          #If "computeBoth" == "computeMap" || "computeBoth" == "computeBoth"
                      X(i,axis1)=aOverB*radius*cosa+x0;
                      X(i,axis2)=       radius*sina+y0;
           //            #If "2" == "3"
           //          #If "computeBoth" == "computeMapDerivative" || "computeBoth" == "computeBoth"  
                      XR(i,axis1,axis1)=-aOverB*radius*scale*sina;
                      XR(i,axis2,axis1)=        radius*scale*cosa;
                      XR(i,axis1,axis2)=aOverB*rad*cosa;
                      XR(i,axis2,axis2)=       rad*sina;
           //            #If "2" == "3"
              }
            }
        else
      // compute(3,computeBoth);
            {
              real angle,radius,sina,cosa;
              for( int i=base; i<=bound; i++ )
              {
                  angle=scale*R(i,axis1)+twoPi*startAngle;
                  radius=rad*R(i,axis2)+innerRadius;
                  sina=sin(angle);
                  cosa=cos(angle);
         //          #If "computeBoth" == "computeMap" || "computeBoth" == "computeBoth"
                      X(i,axis1)=aOverB*radius*cosa+x0;
                      X(i,axis2)=       radius*sina+y0;
           //            #If "3" == "3"
                          X(i,axis3)=zLevel;
             //          #If "computeBoth" == "computeMapDerivative" || "computeBoth" == "computeBoth"  
                      XR(i,axis1,axis1)=-aOverB*radius*scale*sina;
                      XR(i,axis2,axis1)=        radius*scale*cosa;
                      XR(i,axis1,axis2)=aOverB*rad*cosa;
                      XR(i,axis2,axis2)=       rad*sina;
           //            #If "3" == "3"
                          XR(i,axis3,0)=0.;
                          XR(i,axis3,1)=0.;
              }
            }
    }
    else if( computeMap )
    {
        if( rangeDimension==2 )
      // compute(2,computeMap);
            {
              real angle,radius,sina,cosa;
              for( int i=base; i<=bound; i++ )
              {
                  angle=scale*R(i,axis1)+twoPi*startAngle;
                  radius=rad*R(i,axis2)+innerRadius;
                  sina=sin(angle);
                  cosa=cos(angle);
         //          #If "computeMap" == "computeMap" || "computeMap" == "computeBoth"
                      X(i,axis1)=aOverB*radius*cosa+x0;
                      X(i,axis2)=       radius*sina+y0;
           //            #If "2" == "3"
           //          #If "computeMap" == "computeMapDerivative" || "computeMap" == "computeBoth"  
              }
            }
        else
      // compute(3,computeMap);
            {
              real angle,radius,sina,cosa;
              for( int i=base; i<=bound; i++ )
              {
                  angle=scale*R(i,axis1)+twoPi*startAngle;
                  radius=rad*R(i,axis2)+innerRadius;
                  sina=sin(angle);
                  cosa=cos(angle);
         //          #If "computeMap" == "computeMap" || "computeMap" == "computeBoth"
                      X(i,axis1)=aOverB*radius*cosa+x0;
                      X(i,axis2)=       radius*sina+y0;
           //            #If "3" == "3"
                          X(i,axis3)=zLevel;
             //          #If "computeMap" == "computeMapDerivative" || "computeMap" == "computeBoth"  
              }
            }
    }
    else if( computeMapDerivative )
    {
        if( rangeDimension==2 )
      // compute(2,computeMapDerivative);
            {
              real angle,radius,sina,cosa;
              for( int i=base; i<=bound; i++ )
              {
                  angle=scale*R(i,axis1)+twoPi*startAngle;
                  radius=rad*R(i,axis2)+innerRadius;
                  sina=sin(angle);
                  cosa=cos(angle);
         //          #If "computeMapDerivative" == "computeMap" || "computeMapDerivative" == "computeBoth"
         //          #If "computeMapDerivative" == "computeMapDerivative" || "computeMapDerivative" == "computeBoth"  
                      XR(i,axis1,axis1)=-aOverB*radius*scale*sina;
                      XR(i,axis2,axis1)=        radius*scale*cosa;
                      XR(i,axis1,axis2)=aOverB*rad*cosa;
                      XR(i,axis2,axis2)=       rad*sina;
           //            #If "2" == "3"
              }
            }
        else
      // compute(3,computeMapDerivative);
            {
              real angle,radius,sina,cosa;
              for( int i=base; i<=bound; i++ )
              {
                  angle=scale*R(i,axis1)+twoPi*startAngle;
                  radius=rad*R(i,axis2)+innerRadius;
                  sina=sin(angle);
                  cosa=cos(angle);
         //          #If "computeMapDerivative" == "computeMap" || "computeMapDerivative" == "computeBoth"
         //          #If "computeMapDerivative" == "computeMapDerivative" || "computeMapDerivative" == "computeBoth"  
                      XR(i,axis1,axis1)=-aOverB*radius*scale*sina;
                      XR(i,axis2,axis1)=        radius*scale*cosa;
                      XR(i,axis1,axis2)=aOverB*rad*cosa;
                      XR(i,axis2,axis2)=       rad*sina;
           //            #If "3" == "3"
                          XR(i,axis3,0)=0.;
                          XR(i,axis3,1)=0.;
              }
            }
    }
#undef X
#undef X
#undef XR

}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//
//        angle = scale*r(0) + tpi*startAngle
//        radius = rad*r(1) + innerRadius
//        x = aOverB*radius*cos(angle) + x0         (1)
//        y =        radius*sin(angle) + y0         (2)
//        
//        dx/d(r0) = -aOverB*radius*scale*sin(angle)
//        dy/d(r0) =        +radius*scale*cos(angle)
//        dx/d(r1) = aOverB*rad*cos(angle)
//        dy/d(r1) =        rad*sin(angle)
//
//  Inverse:
//        angle = atan2(y-y0,[x-x0]/aOverB)+pi
//        r(0) = (angle-tpi*startAngle)/scale
//        radius = sqrt( ([x-x0]/aOverB)^2 + (y-y0)^2 )    (3)
//        r(1) = (radius-innerRadius)/rad   
//
//   (3) -> 2*radius*rad*dr1/dx = 2*(x-x0)/aOverB**2 -> 
//         dr1/dx = (x-x0)/(rad*radius)/aOverB**2
//         dr1/dy = (y-y0)/(rad*radius)
//                                            
//   d(1)/dx -> 1 = rad*dr1/dx*cos(angle) - radius*sin(angle)*scale*dr0/dx
//              sin(angle)^2 = (y0-y)*scale*dr0/dx
//          dr0/dx = (y0-y)/( radius^2 * scale )/aOverB
//          dr0/dy = (x-x0)/( radius^2 * scale )/aOverB
//        
//=================================================================================
// ====================================================================
// RANGEDIM: 2,3
// OPTION: computeMap,computeMapDerivative,computeBoth
// ISPERIODIC: periodic, notPeriodic
// ====================================================================

void AnnulusMapping:: 
basicInverse( const realArray & x_, realArray & r_, realArray & rx_, MappingParameters & params )
// =========================================================================================
// =========================================================================================
{
    Index I = getIndex( x_,r_,rx_,base,bound,computeMap,computeMapDerivative );

    #ifndef USE_PPP
        realSerialArray & r = r_;
        const realSerialArray & x = x_;
        realSerialArray & rx = rx_;
    #else
        const realSerialArray & r = r_.getLocalArray();
        const realSerialArray & x = x_.getLocalArray();
        const realSerialArray & rx = rx_.getLocalArray();
        base =max(I.getBase(), x.getBase(0));
        bound=min(I.getBound(),x.getBound(0));
        I=Range(base,bound);
    #endif


    real theta0=twoPi*startAngle, theta1=twoPi*endAngle;

    real inverseScale=1./(theta1-theta0);
    real rad=outerRadius-innerRadius;
    real inverseRad=1./rad;

    real * rp = r.Array_Descriptor.Array_View_Pointer1;
    const int rDim0=r.getRawDataSize(0);
#undef R
#define R(i0,i1) rp[i0+rDim0*(i1)]
    const real * xp = x.Array_Descriptor.Array_View_Pointer1;
    const int xDim0=x.getRawDataSize(0);
#undef X
#define X(i0,i1) xp[i0+xDim0*(i1)]
    real * rxp = rx.Array_Descriptor.Array_View_Pointer2;
    const int rxDim0=rx.getRawDataSize(0);
    const int rxDim1=rx.getRawDataSize(1);
#undef RX
#define RX(i0,i1,i2) rxp[i0+rxDim0*(i1+rxDim1*(i2))]

    if( getIsPeriodic(axis1) )
    {
        if( computeMap && computeMapDerivative )
        {
            if( rangeDimension==2 )
      // 	computeInverse( 2,computeBoth,periodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeBoth" == "computeMap" || "computeBoth" == "computeBoth"
         //            #If "periodic" == "periodic" 
                          r1=atan2(double(-y1),double(-x1));  // **NOTE** (-y,-x) : result in [-pi,pi]
                          r1=( r1+(Pi-theta0) )*inverseScale;
                          R(i,axis1)=fmod(r1+1.,1.);  // map back to [0,1]
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeBoth" == "computeMapDerivative" || "computeBoth" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "2" == "3"
              }
            }
            else
      // 	computeInverse( 3,computeBoth,periodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeBoth" == "computeMap" || "computeBoth" == "computeBoth"
         //            #If "periodic" == "periodic" 
                          r1=atan2(double(-y1),double(-x1));  // **NOTE** (-y,-x) : result in [-pi,pi]
                          r1=( r1+(Pi-theta0) )*inverseScale;
                          R(i,axis1)=fmod(r1+1.,1.);  // map back to [0,1]
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeBoth" == "computeMapDerivative" || "computeBoth" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "3" == "3"
                  	RX(i,0,axis3)=0.;
                  	RX(i,1,axis3)=0.;
              }
            }
        }
        else if( computeMap )
        {
            if( rangeDimension==2 )
      // 	computeInverse( 2,computeMap,periodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMap" == "computeMap" || "computeMap" == "computeBoth"
         //            #If "periodic" == "periodic" 
                          r1=atan2(double(-y1),double(-x1));  // **NOTE** (-y,-x) : result in [-pi,pi]
                          r1=( r1+(Pi-theta0) )*inverseScale;
                          R(i,axis1)=fmod(r1+1.,1.);  // map back to [0,1]
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeMap" == "computeMapDerivative" || "computeMap" == "computeBoth"
              }
            }
            else
      // 	computeInverse( 3,computeMap,periodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMap" == "computeMap" || "computeMap" == "computeBoth"
         //            #If "periodic" == "periodic" 
                          r1=atan2(double(-y1),double(-x1));  // **NOTE** (-y,-x) : result in [-pi,pi]
                          r1=( r1+(Pi-theta0) )*inverseScale;
                          R(i,axis1)=fmod(r1+1.,1.);  // map back to [0,1]
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeMap" == "computeMapDerivative" || "computeMap" == "computeBoth"
              }
            }
        }
        else if( computeMapDerivative )
        {
            if( rangeDimension==2 )
      // 	computeInverse( 2,computeMapDerivative,periodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMapDerivative" == "computeMap" || "computeMapDerivative" == "computeBoth"
         //          #If "computeMapDerivative" == "computeMapDerivative" || "computeMapDerivative" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "2" == "3"
              }
            }
            else
      // 	computeInverse( 3,computeMapDerivative,periodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMapDerivative" == "computeMap" || "computeMapDerivative" == "computeBoth"
         //          #If "computeMapDerivative" == "computeMapDerivative" || "computeMapDerivative" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "3" == "3"
                  	RX(i,0,axis3)=0.;
                  	RX(i,1,axis3)=0.;
              }
            }
        }
    }
    else
    {
        if( computeMap && computeMapDerivative )
        {
            if( rangeDimension==2 )
      // 	computeInverse( 2,computeBoth,notPeriodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeBoth" == "computeMap" || "computeBoth" == "computeBoth"
         //            #If "notPeriodic" == "periodic" 
         //            #Else
             // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
             // delta is the gap in the angle between the start and end of the annulus
                          r1=atan2(double(y1),double(x1));  // **NOTE** +theta : result in [-pi,pi]
                          if( r1 < theta0 - delta )
                          {
                              r1+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
                          }
                          R(i,axis1)=(r1-theta0)*inverseScale;
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeBoth" == "computeMapDerivative" || "computeBoth" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "2" == "3"
              }
            }
            else
      // 	computeInverse( 3,computeBoth,notPeriodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeBoth" == "computeMap" || "computeBoth" == "computeBoth"
         //            #If "notPeriodic" == "periodic" 
         //            #Else
             // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
             // delta is the gap in the angle between the start and end of the annulus
                          r1=atan2(double(y1),double(x1));  // **NOTE** +theta : result in [-pi,pi]
                          if( r1 < theta0 - delta )
                          {
                              r1+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
                          }
                          R(i,axis1)=(r1-theta0)*inverseScale;
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeBoth" == "computeMapDerivative" || "computeBoth" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "3" == "3"
                  	RX(i,0,axis3)=0.;
                  	RX(i,1,axis3)=0.;
              }
            }
        }
        else if( computeMap )
        {
            if( rangeDimension==2 )
      // 	computeInverse( 2,computeMap,notPeriodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMap" == "computeMap" || "computeMap" == "computeBoth"
         //            #If "notPeriodic" == "periodic" 
         //            #Else
             // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
             // delta is the gap in the angle between the start and end of the annulus
                          r1=atan2(double(y1),double(x1));  // **NOTE** +theta : result in [-pi,pi]
                          if( r1 < theta0 - delta )
                          {
                              r1+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
                          }
                          R(i,axis1)=(r1-theta0)*inverseScale;
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeMap" == "computeMapDerivative" || "computeMap" == "computeBoth"
              }
            }
            else
      // 	computeInverse( 3,computeMap,notPeriodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMap" == "computeMap" || "computeMap" == "computeBoth"
         //            #If "notPeriodic" == "periodic" 
         //            #Else
             // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
             // delta is the gap in the angle between the start and end of the annulus
                          r1=atan2(double(y1),double(x1));  // **NOTE** +theta : result in [-pi,pi]
                          if( r1 < theta0 - delta )
                          {
                              r1+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
                          }
                          R(i,axis1)=(r1-theta0)*inverseScale;
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeMap" == "computeMapDerivative" || "computeMap" == "computeBoth"
              }
            }
        }
        else if( computeMapDerivative )
        {
            if( rangeDimension==2 )
      // 	computeInverse( 2,computeMapDerivative,notPeriodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMapDerivative" == "computeMap" || "computeMapDerivative" == "computeBoth"
         //          #If "computeMapDerivative" == "computeMapDerivative" || "computeMapDerivative" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "2" == "3"
              }
            }
            else
      // 	computeInverse( 3,computeMapDerivative,notPeriodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMapDerivative" == "computeMap" || "computeMapDerivative" == "computeBoth"
         //          #If "computeMapDerivative" == "computeMapDerivative" || "computeMapDerivative" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "3" == "3"
                  	RX(i,0,axis3)=0.;
                  	RX(i,1,axis3)=0.;
              }
            }
        }
    }
        
#undef X
#undef X
#undef RX
    
}

void AnnulusMapping:: 
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
// =========================================================================================
//   new *serial-array version*
// =========================================================================================
{
    Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

    real theta0=twoPi*startAngle, theta1=twoPi*endAngle;

    real inverseScale=1./(theta1-theta0);
    real rad=outerRadius-innerRadius;
    real inverseRad=1./rad;

    real * rp = r.Array_Descriptor.Array_View_Pointer1;
    const int rDim0=r.getRawDataSize(0);
#undef R
#define R(i0,i1) rp[i0+rDim0*(i1)]
    const real * xp = x.Array_Descriptor.Array_View_Pointer1;
    const int xDim0=x.getRawDataSize(0);
#undef X
#define X(i0,i1) xp[i0+xDim0*(i1)]
    real * rxp = rx.Array_Descriptor.Array_View_Pointer2;
    const int rxDim0=rx.getRawDataSize(0);
    const int rxDim1=rx.getRawDataSize(1);
#undef RX
#define RX(i0,i1,i2) rxp[i0+rxDim0*(i1+rxDim1*(i2))]

    if( getIsPeriodic(axis1) )
    {
        if( computeMap && computeMapDerivative )
        {
            if( rangeDimension==2 )
      // 	computeInverse( 2,computeBoth,periodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeBoth" == "computeMap" || "computeBoth" == "computeBoth"
         //            #If "periodic" == "periodic" 
                          r1=atan2(double(-y1),double(-x1));  // **NOTE** (-y,-x) : result in [-pi,pi]
                          r1=( r1+(Pi-theta0) )*inverseScale;
                          R(i,axis1)=fmod(r1+1.,1.);  // map back to [0,1]
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeBoth" == "computeMapDerivative" || "computeBoth" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "2" == "3"
              }
            }
            else
      // 	computeInverse( 3,computeBoth,periodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeBoth" == "computeMap" || "computeBoth" == "computeBoth"
         //            #If "periodic" == "periodic" 
                          r1=atan2(double(-y1),double(-x1));  // **NOTE** (-y,-x) : result in [-pi,pi]
                          r1=( r1+(Pi-theta0) )*inverseScale;
                          R(i,axis1)=fmod(r1+1.,1.);  // map back to [0,1]
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeBoth" == "computeMapDerivative" || "computeBoth" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "3" == "3"
                  	RX(i,0,axis3)=0.;
                  	RX(i,1,axis3)=0.;
              }
            }
        }
        else if( computeMap )
        {
            if( rangeDimension==2 )
      // 	computeInverse( 2,computeMap,periodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMap" == "computeMap" || "computeMap" == "computeBoth"
         //            #If "periodic" == "periodic" 
                          r1=atan2(double(-y1),double(-x1));  // **NOTE** (-y,-x) : result in [-pi,pi]
                          r1=( r1+(Pi-theta0) )*inverseScale;
                          R(i,axis1)=fmod(r1+1.,1.);  // map back to [0,1]
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeMap" == "computeMapDerivative" || "computeMap" == "computeBoth"
              }
            }
            else
      // 	computeInverse( 3,computeMap,periodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMap" == "computeMap" || "computeMap" == "computeBoth"
         //            #If "periodic" == "periodic" 
                          r1=atan2(double(-y1),double(-x1));  // **NOTE** (-y,-x) : result in [-pi,pi]
                          r1=( r1+(Pi-theta0) )*inverseScale;
                          R(i,axis1)=fmod(r1+1.,1.);  // map back to [0,1]
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeMap" == "computeMapDerivative" || "computeMap" == "computeBoth"
              }
            }
        }
        else if( computeMapDerivative )
        {
            if( rangeDimension==2 )
      // 	computeInverse( 2,computeMapDerivative,periodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMapDerivative" == "computeMap" || "computeMapDerivative" == "computeBoth"
         //          #If "computeMapDerivative" == "computeMapDerivative" || "computeMapDerivative" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "2" == "3"
              }
            }
            else
      // 	computeInverse( 3,computeMapDerivative,periodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMapDerivative" == "computeMap" || "computeMapDerivative" == "computeBoth"
         //          #If "computeMapDerivative" == "computeMapDerivative" || "computeMapDerivative" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "3" == "3"
                  	RX(i,0,axis3)=0.;
                  	RX(i,1,axis3)=0.;
              }
            }
        }
    }
    else
    {
        if( computeMap && computeMapDerivative )
        {
            if( rangeDimension==2 )
      // 	computeInverse( 2,computeBoth,notPeriodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeBoth" == "computeMap" || "computeBoth" == "computeBoth"
         //            #If "notPeriodic" == "periodic" 
         //            #Else
             // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
             // delta is the gap in the angle between the start and end of the annulus
                          r1=atan2(double(y1),double(x1));  // **NOTE** +theta : result in [-pi,pi]
                          if( r1 < theta0 - delta )
                          {
                              r1+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
                          }
                          R(i,axis1)=(r1-theta0)*inverseScale;
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeBoth" == "computeMapDerivative" || "computeBoth" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "2" == "3"
              }
            }
            else
      // 	computeInverse( 3,computeBoth,notPeriodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeBoth" == "computeMap" || "computeBoth" == "computeBoth"
         //            #If "notPeriodic" == "periodic" 
         //            #Else
             // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
             // delta is the gap in the angle between the start and end of the annulus
                          r1=atan2(double(y1),double(x1));  // **NOTE** +theta : result in [-pi,pi]
                          if( r1 < theta0 - delta )
                          {
                              r1+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
                          }
                          R(i,axis1)=(r1-theta0)*inverseScale;
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeBoth" == "computeMapDerivative" || "computeBoth" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "3" == "3"
                  	RX(i,0,axis3)=0.;
                  	RX(i,1,axis3)=0.;
              }
            }
        }
        else if( computeMap )
        {
            if( rangeDimension==2 )
      // 	computeInverse( 2,computeMap,notPeriodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMap" == "computeMap" || "computeMap" == "computeBoth"
         //            #If "notPeriodic" == "periodic" 
         //            #Else
             // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
             // delta is the gap in the angle between the start and end of the annulus
                          r1=atan2(double(y1),double(x1));  // **NOTE** +theta : result in [-pi,pi]
                          if( r1 < theta0 - delta )
                          {
                              r1+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
                          }
                          R(i,axis1)=(r1-theta0)*inverseScale;
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeMap" == "computeMapDerivative" || "computeMap" == "computeBoth"
              }
            }
            else
      // 	computeInverse( 3,computeMap,notPeriodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMap" == "computeMap" || "computeMap" == "computeBoth"
         //            #If "notPeriodic" == "periodic" 
         //            #Else
             // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
             // delta is the gap in the angle between the start and end of the annulus
                          r1=atan2(double(y1),double(x1));  // **NOTE** +theta : result in [-pi,pi]
                          if( r1 < theta0 - delta )
                          {
                              r1+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
                          }
                          R(i,axis1)=(r1-theta0)*inverseScale;
                        R(i,axis2)=(radius-innerRadius)*inverseRad;
            //          #If "computeMap" == "computeMapDerivative" || "computeMap" == "computeBoth"
              }
            }
        }
        else if( computeMapDerivative )
        {
            if( rangeDimension==2 )
      // 	computeInverse( 2,computeMapDerivative,notPeriodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMapDerivative" == "computeMap" || "computeMapDerivative" == "computeBoth"
         //          #If "computeMapDerivative" == "computeMapDerivative" || "computeMapDerivative" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "2" == "3"
              }
            }
            else
      // 	computeInverse( 3,computeMapDerivative,notPeriodic);
            {
              real angle,radius,radiusi,x1,y1,r1;
              real delta = (1.-(endAngle-startAngle))*Pi;
              real ai = 1./aOverB;
              for( int i=base; i<=bound; i++ )
              {
                  x1=(X(i,axis1)-x0)*ai;
                  y1= X(i,axis2)-y0;
                  radius=sqrt(x1*x1+y1*y1);
         //          #If "computeMapDerivative" == "computeMap" || "computeMapDerivative" == "computeBoth"
         //          #If "computeMapDerivative" == "computeMapDerivative" || "computeMapDerivative" == "computeBoth"
                      RX(i,axis2,axis1)= (x1/(rad*radius))*ai;
                      RX(i,axis2,axis2)=  y1/(rad*radius);
                      radiusi=inverseScale/(radius*radius);    
                      RX(i,axis1,axis1)=-y1*radiusi*ai;
                      RX(i,axis1,axis2)= x1*radiusi;
           //            #If "3" == "3"
                  	RX(i,0,axis3)=0.;
                  	RX(i,1,axis3)=0.;
              }
            }
        }
    }
        
#undef X
#undef X
#undef RX
    
}

AnnulusMapping & AnnulusMapping::
operator =( const AnnulusMapping & X0 )
{
    if( AnnulusMapping::className != X0.getClassName() )
    {
        cout << "AnnulusMapping::operator= ERROR trying to set a AnnulusMapping = to a" 
            << " mapping of type " << X0.getClassName() << endl;
        return *this;
    }
    this->Mapping::operator=(X0);            // call = for derivee class
    AnnulusMapping & X = (AnnulusMapping&) X0;  // cast to a Annulus mapping
    x0=X.x0;
    y0=X.y0;
    innerRadius=X.innerRadius;
    outerRadius=X.outerRadius;
    aOverB=X.aOverB;
    startAngle=X.startAngle;
    endAngle=X.endAngle;
    zLevel=X.zLevel;
    
    return *this;
}


// get a mapping from the database
int AnnulusMapping::
get( const GenericDataBase & dir, const aString & name)
{
    GenericDataBase & subDir = *dir.virtualConstructor();
    dir.find(subDir,name,"Mapping");

    if( debug & 4 )
        cout << "Entering AnnulusMapping::get" << endl;

    subDir.get( AnnulusMapping::className,"className" );  
    subDir.get( x0,"x0" );
    subDir.get( y0,"y0" );
    subDir.get( innerRadius,"innerRadius" );
    subDir.get( outerRadius,"outerRadius" );
    subDir.get( aOverB,"aOverB" );
    subDir.get( startAngle,"startAngle" );
    subDir.get( endAngle,"endAngle" );
    subDir.get( zLevel,"zLevel" );
    Mapping::get( subDir, "Mapping" );

    mappingHasChanged();

    delete &subDir;
    return 0;
}
int AnnulusMapping::
put( GenericDataBase & dir, const aString & name) const
{  
    GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
    dir.create(subDir,name,"Mapping");                      // create a sub-directory 

    subDir.put( AnnulusMapping::className,"className" );
    subDir.put( x0,"x0" );
    subDir.put( y0,"y0" );            
    subDir.put( innerRadius,"innerRadius" );
    subDir.put( outerRadius,"outerRadius" );
    subDir.put( aOverB,"aOverB" );
    subDir.put( startAngle,"startAngle" );
    subDir.put( endAngle,"endAngle" );
    subDir.put( zLevel,"zLevel" );
    Mapping::put( subDir, "Mapping" );

    delete &subDir;
    return 0;
}

Mapping* AnnulusMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the className is the name of this Class
    Mapping *retval=0;
    if( AnnulusMapping::className==mappingClassName )
        retval = new AnnulusMapping();
    return retval;
}

//\begin{>>AnnulusMappingInclude.tex}{\subsection{setRadii}}
int AnnulusMapping::
setRadii(const real & innerRadius_ /* =.5 */, 
       	 const real & outerRadius_ /* =1. */,
                  const real aOverB_ /* =1. */  )
//===========================================================================
// /Purpose: Define the radii of the annulus.
// /innerRadius,outerRadius (input): inner and outer radii of the annulus.
//    There is NO restriction that ${\tt innerRadius} < {\tt outerRadius}$.
// /aOverB (input): The ratio of the length of the horizontal-radius ("a") to the vertical-radius ("b")
//  for an elliptical boundary. A value of aOverB=1 defines a circular boundary. 
//\end{AnnulusMappingInclude.tex}
//===========================================================================
{
    innerRadius=innerRadius_;
    outerRadius=outerRadius_;
    aOverB=aOverB_;
    if( innerRadius==0. )
        setTypeOfCoordinateSingularity(Start,axis2,polarSingularity);
    if( outerRadius==0. )
        setTypeOfCoordinateSingularity(End,axis2,polarSingularity);

    signForJacobian= (outerRadius-innerRadius)*(endAngle-startAngle) > 0. ? -1. : 1.;
    return 0;
}

//\begin{>>AnnulusMappingInclude.tex}{\subsection{setOrigin}}
int AnnulusMapping::
setOrigin(const real & x0_ /* =0. */, 
        	  const real & y0_ /* =0. */, 
        	  const real & z0_ /* =0. */ )
//===========================================================================
// /Purpose: Set the centre of the annulus. Choosing a non-zero value for
//  {\tt z0} will cause the {\tt rangeDimension} of the Mapping to become 3.
//  
// /x0,y0,z0 (input): centre of the annulus.
//\end{AnnulusMappingInclude.tex}
//===========================================================================
{
    x0=x0_;
    y0=y0_;
    zLevel=z0_;
    if( zLevel!=0. )
        setRangeDimension(3);
    return 0;
}

//\begin{>>AnnulusMappingInclude.tex}{\subsection{setAngleBounds}}
int AnnulusMapping::
setAngleBounds(const real & startAngle_ /* =0. */, 
             	       const real & endAngle_ /* =1. */ )
//===========================================================================
// /Purpose: Set the angular bounds on the annulus.
// /startAngle, endAngle (input): The initial and final "angle" (in the range [0,1]).
//\end{AnnulusMappingInclude.tex}
//===========================================================================
{
    startAngle=startAngle_;
    endAngle=endAngle_;
  // If the annulus is closed then it becomes periodic
    if( fabs(endAngle-startAngle-1.)<REAL_EPSILON*10. )  // Is the annulus closed
    {
    // printf("setAngleBounds:info setting periodicity to functionPeriodic along axis1 \n");
        setIsPeriodic(axis1,functionPeriodic );  
        setBoundaryCondition( Start,axis1,-1 );
        setBoundaryCondition(   End,axis1,-1 );
    }	
    else
    {
    // printf("setAngleBounds:info setting periodicity to notPeriodic along axis1 \n");
        setIsPeriodic(axis1,notPeriodic );  
        if( getBoundaryCondition(Start,axis1)<=0 )
        {
            setBoundaryCondition(Start,axis1,3);
            setBoundaryCondition(  End,axis1,4);
        }
    }
    signForJacobian= (outerRadius-innerRadius)*(endAngle-startAngle) > 0. ? 1. : -1.;
    return 0;
}



aString AnnulusMapping::
getClassName() const
{
    return AnnulusMapping::className;
}

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int AnnulusMapping::
update( MappingInformation & mapInfo ) 
{

    assert(mapInfo.graphXInterface!=NULL);
    GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
    
    char buff[180];  // buffer for sprintf

  // Here is the old menu: (keep for compatibility)
    aString menu[] = 
        {
            "!AnnulusMapping",
            "centre for annulus",
            "inner radius",
            "outer radius",
            "inner and outer radii",
            "ellipse ratio",
            "start and end angles",
            "make 3d (toggle)",
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
            "centre for annulus : Specify (x0,y0) for the centre",
            "inner radius       : Specify the inner radius",
            "outer radius       : Specify the outer radius",
            "inner and outer radii: specify both inner and outer radii",
            "ellipse ratio      : Enter a/b, the ellipse ratio",
            "start and end angle: Set the start and end values of the angular variable, which is periodic on [0,1]",
            "make 3d (toggle)   : make a 2D grid a surface in 3d or vice versa",
            "lines              : specify number of grid lines",
            "boundary conditions: specify boundary conditions",
            "share              : specify share values for sides",
            "mappingName        : specify the name of this mapping",
            "periodicity        : specify periodicity in each direction",
            "check              : check the mapping and derivatives",
            "check inverse      : input points to check the inverse",
            "show parameters    : print current values for parameters",
            "plot               : enter plot menu (for changing ploting options)",
            "help               : Print this list",
            "exit               : Finished with changes",
            "" 
        };

    bool makeThreeDimensional=rangeDimension==3;

    GUIState dialog;

    dialog.setWindowTitle("Annulus Mapping");
    dialog.setExitCommand("exit", "exit");

  // option menus
//     dialog.setOptionMenuColumns(1);

//     aString opCommand1[] = {"unit square",
// 			    "helical wire",
//                             "fillet for two cylinders",
//                             "blade",
// 			    ""};
        
//     dialog.addOptionMenu( "type:", opCommand1, opCommand1, mappingType); 


    aString colourBoundaryCommands[] = { "colour by bc",
                                 			               "colour by share",
                                 			               "" };
  // dialog.addRadioBox("boundaries:",colourBoundaryCommands, colourBoundaryCommands, 0 );
    dialog.addOptionMenu("boundaries:",colourBoundaryCommands, colourBoundaryCommands, 0 );

    aString cmds[] = {"mapping parameters...",
                		    "show parameters",
                                        "plot",
                		    "help",
                		    ""};
    int numberOfPushButtons=3;  // number of entries in cmds
    int numRows=numberOfPushButtons; // (numberOfPushButtons+1)/2;
    dialog.setPushButtons( cmds, cmds, numRows ); 

    aString tbCommands[] = {"three dimensional",
                      			  ""};
    int tbState[10];

    tbState[0] = makeThreeDimensional;
    int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

    const int numberOfTextStrings=7;  // max number allowed
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];


    int nt=0;
    textLabels[nt] = "center:";  sPrintF(textStrings[nt],"%g, %g",x0,y0);  nt++; 
    textLabels[nt] = "radii:";  sPrintF(textStrings[nt],"%g, %g",innerRadius,outerRadius);  nt++; 
    textLabels[nt] = "angles:";  sPrintF(textStrings[nt],"%g, %g",startAngle,endAngle);  nt++; 
    textLabels[nt] = "ellipse ratio:";  sPrintF(textStrings[nt],"%g",aOverB);  nt++; 

  // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    dialog.setTextBoxes(textLabels, textLabels, textStrings);


  // make a dialog sibling for setting general mapping parameters
    DialogData & mappingParametersDialog = dialog.getDialogSibling();
    buildMappingParametersDialog( mappingParametersDialog );

    dialog.buildPopup(menu);
    gi.pushGUI(dialog);

    int len=0;




    aString answer,line; 

    bool plotObject=true;
    GraphicsParameters parameters;
    parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    parameters.set(GI_LABEL_GRIDS_AND_BOUNDARIES,true); // turn on plotting of coloured squares

    gi.appendToTheDefaultPrompt("Annulus>"); // set the default prompt

    for( int it=0;; it++ )
    {
        if( it==0 && plotObject )
            answer="plotObject";
        else
              gi.getAnswer(answer,"");  // gi.getMenuItem(menu,answer);
  

        if( (len=answer.matches("center:")) )
        {
            sScanF(answer(len,answer.length()-1),"%e %e",&x0,&y0);
            if( !gi.isGraphicsWindowOpen() )
                dialog.setTextLabel("center:",sPrintF(answer,"%g, %g",x0,y0));
            mappingHasChanged();
        }
        else if( (len=answer.matches("radii:")) )
        {
            sScanF(answer(len,answer.length()-1),"%e %e",&innerRadius,&outerRadius);
            if( !gi.isGraphicsWindowOpen() )
                dialog.setTextLabel("radii:",sPrintF(answer,"%g, %g,",innerRadius,outerRadius));
            setRadii(innerRadius,outerRadius,aOverB);
            mappingHasChanged();
        }
        else if( (len=answer.matches("angles:")) )
        {
            sScanF(answer(len,answer.length()-1),"%e %e",&startAngle,&endAngle);
            if( !gi.isGraphicsWindowOpen() )
                dialog.setTextLabel("angles:",sPrintF(answer,"%g, %g,",startAngle,endAngle));

            setAngleBounds(startAngle,endAngle);

      // Update the mapping parameters dialog since the BC's and periodicity may have changed
            updateMappingParametersDialog(mappingParametersDialog);

            mappingHasChanged();
        }
        else if( (len=answer.matches("ellipse ratio:")) )
        {
            sScanF(answer(len,answer.length()-1),"%e",&aOverB);
            if( !gi.isGraphicsWindowOpen() )
                dialog.setTextLabel("ellipse ratio:",sPrintF(answer,"%g,",aOverB));
            setRadii(innerRadius,outerRadius,aOverB);
            mappingHasChanged();
        }
        else if( dialog.getToggleValue(answer,"three dimensional",makeThreeDimensional) )
        {
            rangeDimension = makeThreeDimensional ? 3 : 2;
            mappingHasChanged();
        }
        else if( getMappingParametersOption(answer,mappingParametersDialog,gi ) )
        {
      // Changes were made to generic mapping parameters such as lines, BC's, share, periodicity
            printF("Answer=%s found in getMappingParametersOption\n",(const char*)answer);
        }
        else if( answer=="colour by bc" || 
                          answer=="colour by share" )
        {
            if( answer=="colour by bc" )
            {
                parameters.set(GI_BOUNDARY_COLOUR_OPTION,GraphicsParameters::colourByBoundaryCondition);
        // dialog.getRadioBox(0).setCurrentChoice(0);
                dialog.getOptionMenu("boundaries:").setCurrentChoice(0);
            }
            else if( answer=="colour by share" )
            {
                parameters.set(GI_BOUNDARY_COLOUR_OPTION,GraphicsParameters::colourByShare);
        // dialog.getRadioBox(0).setCurrentChoice(1);
                dialog.getOptionMenu("boundaries:").setCurrentChoice(1);
            }
        }

    // old versions: 
        else if( answer=="centre for annulus" ) 
        {
            gi.inputString(line,sPrintF(buff,"Enter (x0,y0) for centre (default=(%e,%e)): ",x0,y0));
            if( line!="" ) sScanF(line,"%e %e",&x0,&y0);
            mappingHasChanged();
        }
        else if( answer=="inner radius" )
        {
            gi.inputString(line,sPrintF(buff,"Enter the inner radius (default=%e): ",innerRadius));
            if( line!="" ) sScanF( line,"%e",&innerRadius);
            setRadii(innerRadius,outerRadius,aOverB);
            
            mappingHasChanged();
        }
        else if( answer=="outer radius" )
        {
            gi.inputString(line,sPrintF(buff,"Enter the outer radius (default=%e): ",outerRadius));
            if( line!="" ) sScanF( line,"%e",&outerRadius);
            setRadii(innerRadius,outerRadius,aOverB);
            mappingHasChanged();
        }
        else if( answer=="inner and outer radii" )
        {
            gi.outputString("INFO: inner radius is allowed to be larger than the outer radius");
            gi.inputString(line,sPrintF(buff,"Enter the inner and outer radii (default=%e,%e): ",
                innerRadius,outerRadius));

            if( line!="" ) sScanF( line,"%e %e",&innerRadius,&outerRadius);
            setRadii(innerRadius,outerRadius,aOverB);

            
            mappingHasChanged();
        }
        else if( answer=="ellipse ratio" )
        {
            gi.inputString(line,sPrintF(buff,"Enter the ellipse ratio aOverB (default=%e): ",aOverB));
            if( line!="" ) sScanF( line,"%e",&aOverB);
            mappingHasChanged();
        }
        else if( answer=="start and end angles" )
        {
            gi.outputString("INFO: Angles are periodic on [0,1), start and end angles may lie in [-.5,1.5]");
            gi.inputString(line,sPrintF(buff,"Enter the starting and ending `angles' (1-periodic) (default=%e,%e): ",
                    startAngle,endAngle));
            if( line!="" ) sScanF( line,"%e %e",&startAngle,&endAngle);

            setAngleBounds(startAngle,endAngle);

      // Update the mapping parameters dialog since the BC's and periodicity may have changed
            updateMappingParametersDialog(mappingParametersDialog);
            mappingHasChanged();
        }
        else if( answer=="make 3d (toggle)" )
        {
            if( rangeDimension==2 )
            {
                rangeDimension=3;
                gi.inputString(line,sPrintF(buff,"Enter the z value for the grid (default=%e): ",zLevel));
                if( line!="" ) sScanF( line,"%e",&zLevel);
            }
            else
            {
                rangeDimension=2;
            }
            mappingHasChanged();
        }
        else if( answer=="lines"  ||
                          answer=="boundary conditions"  ||
                          answer=="share"  ||
                          answer=="mappingName"  ||
                          answer=="periodicity"  ||
                          answer=="check"||
                          answer=="check inverse" )
        { // call the base class to change these parameters:
            mapInfo.commandOption=MappingInformation::readOneCommand;
            mapInfo.command=&answer;
            Mapping::update(mapInfo); 
            mapInfo.commandOption=MappingInformation::interactive;
            if( (bool)getIsPeriodic(axis1) && fabs(endAngle-startAngle-1.)>REAL_EPSILON*10. )
            {
      	printf("***WARNING**** Annulus is periodic but |endAngle-StartAngle| is not 1.\n");
            }
        }
        else if( answer=="show parameters" )
        {
            printf(" (innerRadius,outerRadius=(%e,%e)\n centre: (x0,y0)=(%e,%e)\n",innerRadius,outerRadius,
           	     x0,y0);
            printf(" (startAngle,endAngle)=(%e,%e)\n",startAngle,endAngle);
            if( rangeDimension==3 )
      	printf("zlevel=%e \n",zLevel);
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

    gi.popGUI(); // restore the previous GUI

    return 0;

}

