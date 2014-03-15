#include "SmoothedPolygon.h"
#include "MappingInformation.h"
#include "display.h"
#include <float.h>

#define CPINIT  EXTERN_C_NAME(cpinit)
#define CPRSI   EXTERN_C_NAME(cprsi)
#define CPRG    EXTERN_C_NAME(cprg)
#define STTR   EXTERN_C_NAME(sttr)
#define STRT   EXTERN_C_NAME(strt)

extern "C"
{
  
  void CPINIT( int & ndi, int & iwx, int & iwy, int & iwr, int & iws, int & iwr1, int & ndr, 
               real & rwx, real & rwy, real & rwr, real & rws, real & rwr1, real & ccor,
               int & nc, real & sc, real & xc, real & yc, real & bv, real & sab, real & r12b, 
               int &  nsr, real & sr, int &  ndwk, real & wk, real & x00, real & x01, 
               real & x10, real & x11, real & y00, real & y01, real & y10, real & y11, 
               int & iccor, int & per );

  void CPRSI( int & nr, int & ns, int & per, int & ndw, real & w, real & iw, int & nwdi, 
              int & iwx, int & iwy, int & iwr, int & iws, int & iwr1, int & nwdr, real & rwx, 
              real & rwy, real & rwr, real & rws, real & rwr1, real & ccor );

  void CPRG( real & r, real & s, real & x, real & y, real & xr, real & xs, real & yr, real & ys, 
            int & ndiwk, int & iwk, int &  ndrwk, real & rwk, int &  ierr );

  void STTR( real & t, real & r, real & rt, int & iw, real & rw, int & ierr );
  void STRT( real & r, real & t, real & tr, int & iw, real & rw, int & ierr );

}





SmoothedPolygon::
SmoothedPolygon()
: Mapping(2,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  Default Constructor. It creates a mapping with domainDimension=2,
///   rangeDimension=2, domainSpace=parameterSpace and rangeSpace=cartesianSpace.
//===========================================================================
{ 
  mappingIsDefined=FALSE;
  SmoothedPolygon::className="SmoothedPolygon";
  setName( Mapping::mappingName,"smoothedPolygon");
  setGridDimensions(0,0);
  setGridDimensions(1,0);
  // The smoothed polygon is approximated by a spline:
  // *wdh* 031211 : increased this value from 5 to 6 (for problems with 4th-order accurate mx)
  //  NOTE: for some reason even factors give a better answer for the derivatives!
  splineResolutionFactor=6.; // numberOfSplinePoints=splineResolutionFactor*numberOfGridsPoints
  
  // mapIsDistributed=true;  // ** for testing **********************************************************

  setDefaultValues();
  mappingHasChanged();
}


// Copy constructor is deep by default
SmoothedPolygon::
SmoothedPolygon( const SmoothedPolygon & map, const CopyType copyType )
{
  SmoothedPolygon::className="SmoothedPolygon";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    printF("SmoothedPolygon:: sorry no shallow copy constructor, doing a deep! \n");
    *this=map;
  }
}

SmoothedPolygon::
~SmoothedPolygon()
{ 
  if( debug & 4 )
    printF(" SmoothedPolygon::Desctructor called\n");
}


void SmoothedPolygon::
setDefaultValues() 
//
// /Purpose: Assign default values for all variables
//
{
  zLevel=0.;
  numberOfVertices=5;  // default value
  vertex.redim(numberOfVertices,2);
  arclength.redim(numberOfVertices);

  if( getGridDimensions(0)<=0 || getGridDimensions(1)<=0 )
  {
    setGridDimensions(0,15*numberOfVertices);   // number of grid lines in tangential
    setGridDimensions(1,7);                    // number of grid lines in normal direction
  }

  // default points are (0,0) (1,0)  (1,1) (0,1)
//vertex=0;  vertex(1,0)=1.; vertex(2,0)=1.; vertex(2,1)=1.; vertex(3,1)=1.; 
//normalDistance=-.1;

  vertex(0,0)=-.05; vertex(0,1)=.25;
  vertex(1,0)= .05; vertex(1,1)= .25;
  vertex(2,0)= .05; vertex(2,1)=-.25;
  vertex(3,0)=-.05; vertex(3,1)=-.25;
  vertex(4,0)=-.05; vertex(4,1)= .25;
  normalDistance=.15;

  setIsPeriodic(axis1,Mapping::functionPeriodic);
  setBoundaryCondition(Start,axis1,-1);
  setBoundaryCondition(End,  axis1,-1);

  //  ...assign default values for parameters
  numberOfRStretch=1;
  sr.redim(3,numberOfRStretch);
  bv.redim(numberOfVertices);
  sab.redim(2,numberOfVertices);
  r12b.redim(3,numberOfVertices);

  int i;
  for( i=0; i<numberOfVertices; i++ )
  {
    bv(i)=40.    ;               // sharpness of corners
    sab(0,i)=.15 ;               // weight : stretching in t direction at vertices
    sab(1,i)=50. ;               // exponent : stretching in t-direction ..
    r12b(0,i)=normalDistance ;   // radius at point i+
    r12b(1,i)=normalDistance ;   // radius at point (i+1)-
    r12b(2,i)=50.;               // transition factor from ? to ?
  }
  
  userStretchedInT=FALSE;        // becomes true when user specifies t-stretch
  sab(0,0) =0.;                  // no stretching at end points by default
  sab(0,numberOfVertices-1)=0.;
  if( !userStretchedInT ) // if user has not specified stretching in the t-direction
  {
    if( getIsPeriodic(axis1)==functionPeriodic )  // set t-stretching at one periodic end
      sab(0,0) =.15;
    else
      sab(0,0) =.0;                            // no t-stretching at end by default
  }	
  
  sr(0,0)=1.;           // default stretching in the r-direction
  sr(1,0)=4.;
  sr(2,0)=0.;

  correctCorners=FALSE;  // correct corners? 0=no
  corner.redim(2,2,2);
  corner=0.;  

}

int SmoothedPolygon::
correctPolygonForCorners()
// ================================================================================
/// \details 
///      Correct the polygon for corners (protected routine).
///  
// ================================================================================
{
  
  corner(axis1,0,0)=vertex(0,0);
  corner(axis2,0,0)=vertex(0,1);
  corner(axis1,1,0)=vertex(numberOfVertices-1,0);
  corner(axis2,1,0)=vertex(numberOfVertices-1,1);

  const real signOfNormalDistance = normalDistance>0. ? 1 : -1.;
      
  // get normals to the polygon:
  int ie=0;  // start pt index
  real nx1 = - (vertex(ie+1,1)-vertex(ie,1));
  real ny1 =   (vertex(ie+1,0)-vertex(ie,0));
  real norm= sqrt( nx1*nx1+ny1*ny1 )*signOfNormalDistance;
  nx1/=norm; ny1/=norm;
      
  ie=numberOfVertices-1;
  real nx2 = - (vertex(ie,1)-vertex(ie-1,1));
  real ny2 =   (vertex(ie,0)-vertex(ie-1,0));
  norm= sqrt( nx2*nx2+ny2*ny2 )*signOfNormalDistance;
  nx2/=norm; ny2/=norm;
      
  // evaluate the mapping to get the normal distance
  realArray r(4,2),x(4,2);	
  r(0,0)=0.; r(0,1)=0.; 
  r(1,0)=0.; r(1,1)=1.; 
  r(2,0)=1.; r(2,1)=0.; 
  r(3,0)=1.; r(3,1)=1.; 
  map(r,x);

  real nDist1 = sqrt( SQR(x(1,0)-x(0,0))+SQR(x(1,1)-x(0,1)) );
  real nDist2 = sqrt( SQR(x(3,0)-x(2,0))+SQR(x(3,1)-x(2,1)) );
	
  corner(axis1,0,1)=corner(axis1,0,0)+nx1*nDist1;
  corner(axis2,0,1)=corner(axis2,0,0)+ny1*nDist1;

  corner(axis1,1,1)=corner(axis1,1,0)+nx2*nDist2;
  corner(axis2,1,1)=corner(axis2,1,0)+ny2*nDist2;

  correctCorners=true;
  // ::display(corner,"corner (new)");

  mappingHasChanged();

  return 0;
}



int SmoothedPolygon::
setPolygon( const RealArray & xv,
            const RealArray & sharpness /* = Overture::nullRealArray() */,
            const real normalDist /* = 0. */,
            const RealArray & variableNormalDist /* = Overture::nullRealArray() */,
            const RealArray & tStretch /* = Overture::nullRealArray() */,
            const RealArray & rStretch /* = Overture::nullRealArray() */,
            const bool correctForCorners /* = false */ )
// ================================================================================
///  
/// \details 
///      Set the verticies for the smooth polygon and optionally set other properties.
///  
///    **finish me for setting other parameters***  
///  
/// \param xv (input) : array of vertices, xv(0:numberOfVertices-1,0:1)
/// \param sharpness (input) : sharpness(0:numberOfVertices-1), if specified, provide the sharpness of each corner.
/// \param normalDist (input) : if non-zero then use this as the fixed normal distance (sign is important).
/// \param variableNormalDist (input) : variableNormalDistance(0:2,0:numberOfVertices-1) if provided this is
///      the variable normal distance and transition exponent.
/// \param tStretch (input) : tStretch(0:1,0:numberOfVertices-1) tangential stretching weight and exponent
/// \param rStretch (input) : rStretch(0:2) radial stretching weight, exponent and location.
/// 
// ================================================================================
{
  numberOfVertices=xv.getLength(0);

  vertex.redim(numberOfVertices,2);

  vertex=xv;

  arclength.redim(numberOfVertices); arclength=0.;
  real ds=0.,dsmax=0.,dsmin=0.;

  if( getGridDimensions(0)<=0 || getGridDimensions(1)<=0 )
  {
    setGridDimensions(0,7*numberOfVertices);   // number of grid lines in tangential
    setGridDimensions(1,7);                    // number of grid lines in normal direction
  }
  for( int i=0; i<numberOfVertices; i++ )
  {
    if( i>0 )
    {
      ds=SQRT( SQR(vertex(i,0)-vertex(i-1,0)) + SQR(vertex(i,1)-vertex(i-1,1)) );
      dsmax= i>1 ? max(dsmax,ds) : ds;
      dsmin= i>1 ? min(dsmin,ds) : ds;
    }
  }

  const bool sharpnessProvided = sharpness.dimension(0)==vertex.dimension(0);
  const bool normalDistProvided = normalDist!=0.;
  const bool variableNormalDistProvided = (variableNormalDist.getLength(0)==3 && 
                                           variableNormalDist.dimension(1)==vertex.dimension(0) );
  const bool tStretchProvided = (tStretch.getLength(0)==2 &&
                                 tStretch.dimension(1)==vertex.dimension(0) );
  const bool rStretchProvided = rStretch.getLength(0)==3;

  //  ...assign values for parameters
  numberOfRStretch=1;
  sr.redim(3,numberOfRStretch);   sr=0.;

  bv.redim(numberOfVertices);     bv=0.;
  sab.redim(2,numberOfVertices);  sab=0.;
  r12b.redim(3,numberOfVertices); r12b=0.;

  if( normalDistProvided )
    normalDistance=normalDist;
  else
    normalDistance=-(dsmax+dsmin)*.05;  // guess normal length to use

  for( int i=0; i<numberOfVertices; i++ )
  {
    bv(i)=sharpnessProvided ? sharpness(i) : 40.;               // sharpness of corners
    if( tStretchProvided )
    {
      sab(0,i)=tStretch(0,i);       // weight : stretching in t direction at vertices
      sab(1,i)=tStretch(1,i);       // exponent : stretching in t-direction ..
    }
    else
    {
      sab(0,i)=.15 ;               // weight : stretching in t direction at vertices
      sab(1,i)= 50.;               // exponent : stretching in t-direction ..
    }
    
    if( variableNormalDistProvided )
    {
      r12b(0,i)=variableNormalDist(0,i);   // radius at point i+
      r12b(1,i)=variableNormalDist(1,i);   // radius at point (i+1)-
      r12b(2,i)=variableNormalDist(2,i);   // transition factor from ? to ?
    }
    else
    {
      r12b(0,i)=normalDistance ;   // radius at point i+
      r12b(1,i)=normalDistance ;   // radius at point (i+1)-
      r12b(2,i)=50.;               // transition factor from ? to ?
    }
    
  }
      
  // if the polygon is not closed, reset periodicity if set
  if( SQR(vertex(numberOfVertices-1,0)-vertex(0,0))+
      SQR(vertex(numberOfVertices-1,1)-vertex(0,1)) > 10.*REAL_EPSILON )
  {
    if( getIsPeriodic(axis1) != Mapping::notPeriodic )
    {
      setIsPeriodic(axis1,Mapping::notPeriodic);
      setBoundaryCondition(Start,axis1,1);
      setBoundaryCondition(End,  axis1,1);
    }
  }

  userStretchedInT=tStretchProvided;        // becomes true when user specifies t-stretch
  if( !userStretchedInT ) // if user has not specified stretching in the t-direction
  {
    sab(0,0) =0.;                  // no stretching at end points by default
    sab(0,numberOfVertices-1)=0.;

    if( getIsPeriodic(axis1)==functionPeriodic )  // set t-stretching at one periodic end
      sab(0,0) =.15;
    else
      sab(0,0) =.0;                            // no t-stretching at end by default
  }	
      
  if( rStretchProvided )
  {
    sr(0,0)=rStretch(0);           // default stretching in the r-direction
    sr(1,0)=rStretch(1);
    sr(2,0)=rStretch(2);
  }
  else
  {
    sr(0,0)=1.;           // default stretching in the r-direction
    sr(1,0)=4.;
    sr(2,0)=0.;
  }
  

  corner.redim(2,2,2);
  corner=0.;  

  mappingHasChanged();
  mappingIsDefined=true;   // the smoothed polygon is now defined

  initialize();

  correctCorners=false;  // Thhis will be changed to true below if correctForCorners==true
  if( correctForCorners )
  {
    correctPolygonForCorners();
    mappingHasChanged();
  }

  return 0;
}



void SmoothedPolygon::
initialize()
//
// /Purpose: Initialize the smoothed polygon routines, given values for the vertices etc.
//
{
  // number of points for spline in t direction
  int numberOfSplinePoints=int( getGridDimensions(axis1)*splineResolutionFactor+.5); 

  const int maxSplinePoints=500;
  if( false && numberOfSplinePoints>maxSplinePoints )
  {
    numberOfSplinePoints=maxSplinePoints; // max(getGridDimensions(axis1),maxSplinePoints);
    printF("Warning: reducing the number of spline points to %i for approximating the SmoothedPolygon curve\n"
           " Too many spline points can reduce the accuracy of the derivatives\n",numberOfSplinePoints);
  }
  else 
  {
    printF("INFO: Using numberOfSplinePoints=%i to approximate the SmoothedPolygon\n",numberOfSplinePoints);
  }
  
  ndi=12+(numberOfVertices)*2;
  ndr=3*(numberOfVertices+1)+4+0+numberOfSplinePoints*5+2*(numberOfVertices)+100;

  int ndwsp=9+9*numberOfSplinePoints; // work space for splines in cprsi

  const int ndccor=20;  // ???
  
  ndiwk=5*ndi+4;
  ndrwk=5*ndr+ndccor+ndwsp;

  iwk.redim(ndiwk); iwk=0;
  rwk.redim(ndrwk); rwk=0.;
  
  // ...........Initialization call for CPR

  int ndwk=max(100,numberOfSplinePoints);
  RealArray wk(ndwk); wk=0.;
  
  iwk(0)=ndi;
  iwk(1)=ndr;
  int periodic=getIsPeriodic(axis1);
  CPINIT( ndi,iwk(4),iwk(4+ndi),iwk(4+2*ndi),
	 iwk(4+3*ndi),iwk(4+4*ndi),
	 ndr,rwk(0),rwk(ndr),rwk(2*ndr),
	 rwk(3*ndr),rwk(4*ndr),
	 rwk(5*ndr),
	 numberOfVertices,arclength(0),vertex(0,0),vertex(0,1),bv(0),sab(0,0),r12b(0,0), 
	 numberOfRStretch,sr(0,0), ndwk,wk(0),
	 corner(0,0,0),corner(0,1,0),corner(0,0,1),corner(0,1,1),
	 corner(1,0,0),corner(1,1,0),corner(1,0,1),corner(1,1,1),
	 correctCorners,periodic );
  //.....Initialize spline routines cprs

  iwk(2)=ndwsp;
  iwk(3)=1+5*ndr+ndccor;
  CPRSI( numberOfSplinePoints,numberOfSplinePoints,periodic, ndwsp,
	rwk(-1+iwk(3)),rwk(-1+iwk(3)),
	ndi,iwk(4),iwk(4+ndi),iwk(4+2*ndi),
	iwk(4+3*ndi),iwk(4+4*ndi),
	ndr,rwk(0),rwk(ndr),rwk(2*ndr),
	rwk(3*ndr),rwk(4*ndr),
	rwk(5*ndr) );

  assignAdoptions();

  mappingIsDefined=true;   // the smoothed polygon is now defined
  mappingHasChanged();
}

void SmoothedPolygon::
assignAdoptions()
// Assign the arrays which are adopted
{
  iw.adopt((int *) &rwk(iwk(3)-1),iwk(2));
  w.adopt( &rwk(iwk(3)-1),iwk(2));
  ndi=iwk(0); // cout << "SP: ndi=" << ndi << endl;
  ndr=iwk(1); // cout << "SP: ndr=" << ndr << endl;
  nsm1 = iw(1)-1; // nsm1=iw(2)-1 //    cout << "SP: nsm1=" << nsm1 << endl;
  bx.adopt(&w(iw(6)-1),3,Range(1,nsm1+1));
  by.adopt(&w(iw(7)-1),3,Range(1,nsm1+1));
  ccor.adopt(&rwk(5*ndr),Range(1,19));
}


SmoothedPolygon & SmoothedPolygon::
operator =( const SmoothedPolygon & X )
{
  if( Mapping::debug & 4 ) 
    printF("SmoothPolygon: operator= called \n");
  
  if( SmoothedPolygon::className != X.getClassName() )
  {
    printF("SmoothedPolygon::operator= ERROR trying to set a SmoothedPolygon = to a" 
           " mapping of type %s\n",(const char*)X.getClassName());
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  mappingIsDefined=X.mappingIsDefined;

  numberOfVertices=X.numberOfVertices;
  vertex.redim(0); vertex=X.vertex;
  arclength.redim(0); arclength=X.arclength;
  NumberOfRStretch=X.NumberOfRStretch;
  userStretchedInT=X.userStretchedInT;
  correctCorners  =X.correctCorners;
  bv.redim(0); bv=X.bv;
  sab.redim(0); sab=X.sab;
  r12b.redim(0); r12b=X.r12b;
  sr.redim(0); sr=X.sr;
  corner.redim(0); corner=X.corner;
  numberOfRStretch=X.numberOfRStretch;
  normalDistance  =X.normalDistance;
  zLevel=X.zLevel;
  splineResolutionFactor=X.splineResolutionFactor;
  
  ndiwk=X.ndiwk;
  ndrwk=X.ndrwk;
  iwk.redim(0); iwk=X.iwk;
  rwk.redim(0); rwk=X.rwk;

  // fix up adoptions
  assignAdoptions();

  return *this;
}



void SmoothedPolygon::
map( const realArray & rIn, realArray & xIn, realArray & xrIn,
     MappingParameters & params )
// ===========================================================================================
// /Description:
//   Evaluate the smoothed polygon mapping
// ===========================================================================================
{
  if( params.coordinateType != cartesian )
    cerr << "SmoothedPolygon::map - coordinateType != cartesian " << endl;

  if( !mappingIsDefined)
     initialize();

  Index I = getIndex( rIn,xIn,xrIn,base,bound,computeMap,computeMapDerivative );

  #ifndef USE_PPP
    const realSerialArray & r_ = rIn;
    realSerialArray & x = xIn;
    realSerialArray & xr = xrIn;
  #else
    const realSerialArray & r_ = rIn.getLocalArray();
    const realSerialArray & x = xIn.getLocalArray();
    const realSerialArray & xr = xrIn.getLocalArray();
    base =max(I.getBase(), r_.getBase(0));
    bound=min(I.getBound(),r_.getBound(0));
    I=Range(base,bound);
  #endif


  int ierr;

  const real BIG_REAL = REAL_MAX*.1;
  
  Range D(0,domainDimension-1);
  realSerialArray r(I,D);
  r=r_(I,D);
  if( getIsPeriodic(axis1)==functionPeriodic )
  {
    r(I,axis1)=fmod(r(I,axis1)+1.,1.);  // map back to [0,1]
  }
  

  const real rMax=.5+2.;   // only evaluate for r in [-2,2]
  const real signOfNormalDistance = normalDistance>0. ? 1 : -1.;
  
  if( domainDimension==2 )
  {

    for( int i=base; i<=bound; i++ )
    {
      if( fabs(r(i,axis1)-.5)>rMax )
      {
	if( computeMap )
	{
	  x(i,axis1)=x(i,axis2)=BIG_REAL;
          if( rangeDimension==3 )
	    x(i,axis3)=BIG_REAL;
	}
	if( computeMapDerivative )
	{
	  xr(i,axis1,axis1)=xr(i,axis2,axis2)=1.;
	  xr(i,axis1,axis2)=xr(i,axis2,axis1)=0.;
          if( rangeDimension==3 )
            xr(i,axis3,Range(0,1))=0.;
	}
      }
      else
      {
	// real s=r(i,axis1);
	int j= int(r(i,axis1)*nsm1+1);
	j=max(min(j,nsm1),1);

	real ds=r(i,axis1)-(j-1)/real(nsm1);

        // evaluate the spline
	real x0  =w(iw(4)-1+j-1)+ds*(bx(0,j)+ds*(bx(1,j)+ds*bx(2,j)));
	real x0s =bx(0,j)+ds*(2.*bx(1,j)+ds*3.*bx(2,j));
	real x0ss=2.*(bx(1,j)+ds*3.*bx(2,j));
	real y0  =w(iw(5)-1+j-1)+ds*(by(0,j)+ds*(by(1,j)+ds*by(2,j)));
	real y0s =by(0,j)+ds*(2.*by(1,j)+ds*3.*by(2,j));
	real y0ss=2.*(by(1,j)+ds*3.*by(2,j));

	

	//.......stretching in s direction (near corners etc.)
	real s0,s0s;
	STRT( r(i,axis1),s0,s0s, iwk(4+3*ndi),rwk(3*ndr),ierr ); // STRT( s,s0,s0s,   iws(0),rws(0),ierr ); 
      
	//.......radial width varies in s direction
	real r0,r0s0;
	STTR( s0,r0,r0s0, iwk(4+2*ndi),rwk(2*ndr),ierr );  //    STTR( s0,r0,r0s0, iwr(0),rwr(0),ierr );
      
	//........lines stretched in r direction
	real r1,r1r;
	STRT(r(i,axis2),r1,r1r,iwk(4+4*ndi),rwk(4*ndr),ierr); //STRT(r(i,axis2),r1,r1r,iwr1(0),rwr1(0),ierr );
      
	real rr0=r1*r0;
	real d=1./SQRT(x0s*x0s+y0s*y0s);
      
        //  Usually the grid is defined by moving in the normal direction from the boundary
        //    x(s,r) = x0(s) + r*d0(s)
        //            d0(s) = (-y0s(s),x0s(s))/|(x0s,y0s)|) * D0
        //  Near corners we may adjust the direction to match the normal to the polygon:
        //
        //    The correction vector is
        //          dc = [  (-yvs,xvs)/|(xvs,yvs)|  - (-y0s(0),x0s(0))/|(x0s(0),y0s(0))| ]
        //      d(s) = d0(s) + dc* D0 * (1-alpha(s)
        // 

        real dx = -rr0*y0s*d;  // direction vector is (dx,dy)
	real dy =  rr0*x0s*d;

	real rr0s=r1*r0s0*s0s;
	real tmp=-(x0s*y0ss-y0s*x0ss)*d*d*d;

	real dxs = rr0*x0s*tmp-rr0s*y0s*d;      // (dxs,dys) : derivative w.r.t. tangential variable s
	real dys = rr0*y0s*tmp+rr0s*x0s*d;
	real dxr = -r1r*r0*y0s*d;               // (dxr,dyr) : derivative w.r.t. the normal variable r
	real dyr = +r1r*r0*x0s*d;

        bool adjustDirection=ccor(18) > 0.5;
        const real rw=.1;  // width of transition
        if( adjustDirection && fabs(r(i,axis1)-.5)>(.5-rw) )
	{
          // Adjust the tangents to match the slope of the final polygon line segment
          int je; 
          real xvs,yvs,xs0,ys0,alpha,alphas,scale,dvs,dcx,dcy;

          // slope of last segment of the polygon is (xvs,yvs)
          if( r(i,axis1)<.5 )
          {
            je=1;  // end point of spline

            // use specified end vector
	    yvs =-(ccor(12)-ccor(10));
	    xvs = (ccor(16)-ccor(14));
            // printF("Start: (xvs,yvs)=(%8.4f,%8.4f) ",xvs,yvs);

	    // xvs = vertex(1,0)-vertex(0,0);   
	    // yvs = vertex(1,1)-vertex(0,1);       
            // printF("old: (xvs,yvs)=(%8.4f,%8.4f) \n",xvs,yvs);

            dvs = r(i,axis1)/rw;
	    // alpha = max(0,r(i,axis1)/rw);
	    // blending function f(x) = x + x^2 - x^3  
	    //                   f'(x)= 1 + 2x - 3x^2
	    //  f(0)=0, f'(0)=1 f(1)=1, f'(1)=0
            // s = r(i,0)
	    alpha= dvs*( 1.+dvs*(1.-dvs) );  
	    alphas= (1.+dvs*(2.-3.*dvs))/rw;
	  }
	  else
	  {
            je=nsm1+1;

            // use specified end vector
	    yvs =-(ccor(13)-ccor(11));
	    xvs = (ccor(17)-ccor(15));
            // printF("End: (xvs,yvs)=(%8.4f,%8.4f) ",xvs,yvs);

	    // xvs = vertex(numberOfVertices-1,0)-vertex(numberOfVertices-2,0);   
	    // yvs = vertex(numberOfVertices-1,1)-vertex(numberOfVertices-2,1);       

            // printF("old: (xvs,yvs)=(%8.4f,%8.4f) \n",xvs,yvs);

            dvs=(1.-r(i,axis1))/rw;
	    alpha= dvs*( 1.+dvs*(1.-dvs) );  
	    alphas= -(1.+dvs*(2.-3.*dvs))/rw;
	  }
          scale=sqrt(xvs*xvs+yvs*yvs);
          xvs/=scale; yvs/=scale;         // normalize direction to the polygon
	  
          // slope of spline at end is (xs0,ys0)
	  xs0 = bx(0,je); // derivative on end
	  ys0 = by(0,je); 
          scale=sqrt(xs0*xs0+ys0*ys0)*signOfNormalDistance;
	  xs0/=scale; ys0/=scale;            // normalize

          dcx = -(yvs - ys0); // here is the (constant) correction vector for direction at the end
          dcy =  (xvs - xs0);

          dx += rr0*dcx*(1.-alpha);   //  add correction to direction 
	  dy += rr0*dcy*(1.-alpha);
	  
          dxs += rr0s*dcx*(1.-alpha) -alphas*rr0*dcx;   // adjust derivatives
          dys += rr0s*dcy*(1.-alpha) -alphas*rr0*dcy;

          dxr += r1r*r0*dcx*(1.-alpha);
          dyr += r1r*r0*dcy*(1.-alpha);
	}

	if( computeMap )
	{
	  // x(i,axis1)=x0-rr0*y0s*d;
	  // x(i,axis2)=y0+rr0*x0s*d;
	  x(i,axis1)=x0+dx;
	  x(i,axis2)=y0+dy;
          if( rangeDimension==3 )
  	    x(i,axis3)=zLevel;
	}
      
	if( computeMapDerivative )
	{
	  // real rr0s=r1*r0s0*s0s;
	  // real tmp=-(x0s*y0ss-y0s*x0ss)*d*d*d;
	  // xr(i,axis1,axis1)=x0s+rr0*x0s*tmp-rr0s*y0s*d;
	  // xr(i,axis2,axis1)=y0s+rr0*y0s*tmp+rr0s*x0s*d;
	  // xr(i,axis1,axis2)=-r1r*r0*y0s*d;
	  // xr(i,axis2,axis2)=+r1r*r0*x0s*d;

	  xr(i,axis1,axis1)=x0s+dxs;
	  xr(i,axis2,axis1)=y0s+dys;
	  xr(i,axis1,axis2)=    dxr;
	  xr(i,axis2,axis2)=    dyr;

          if( rangeDimension==3 )
            xr(i,axis3,Range(0,1))=0.;
	}    

	//.....Now project onto (straight) boundary curves s=0 and s=1 *** is this still needed?

	if( ccor(18) > 0.5 )
	{
	  real epss=ccor(9);
	  // cout << "SP: epss = " << epss << endl;
	  if( fabs(r(i,0))<epss )
	  {
	    if( computeMap )
	    {
	      x(i,0)=ccor(10)+r1*(ccor(12)-ccor(10));
	      x(i,1)=ccor(14)+r1*(ccor(16)-ccor(14));
	    }
	  
	    if( computeMapDerivative )
	    {
	      xr(i,0,1)=(ccor(12)-ccor(10))*r1r;
	      xr(i,1,1)=(ccor(16)-ccor(14))*r1r;
	    }
	  }
	  else if( fabs(r(i,0)-1.)<epss )
	  {
	    if( computeMap )
	    {
	      x(i,0)=ccor(11)+r1*(ccor(13)-ccor(11));
	      x(i,1)=ccor(15)+r1*(ccor(17)-ccor(15));
	    }
	  
	    if( computeMapDerivative )
	    {
	      xr(i,0,1)=(ccor(13)-ccor(11))*r1r;
	      xr(i,1,1)=(ccor(17)-ccor(15))*r1r;
	    }
	  }
	}
      }
    }
  }
  else 
  { // domain dimension==1 : polygon is just a curve

    for( int i=base; i<=bound; i++ )
    {
      if( fabs(r(i,axis1)-.5)>rMax )
      {
	if( computeMap )
	{
	  x(i,axis1)=x(i,axis2)=BIG_REAL;
          if( rangeDimension==3 )
            x(i,axis3)=BIG_REAL;
	}
	if( computeMapDerivative )
	{
	  xr(i,axis1,axis1)=1.;
	  xr(i,axis2,axis1)=0.;
          if( rangeDimension==3 )
            xr(i,axis3,axis1)=0.;
	}
      }
      else
      {
	// real s=r(i,axis1);
	int j= int(r(i,axis1)*nsm1+1);
	j=max(min(j,nsm1),1);

	real ds=r(i,axis1)-(j-1)/real(nsm1);

	real x0  =w(iw(4)-1+j-1)+ds*(bx(0,j)+ds*(bx(1,j)+ds*bx(2,j)));
	real x0s =bx(0,j)+ds*(2.*bx(1,j)+ds*3.*bx(2,j));
	real x0ss=2.*(bx(1,j)+ds*3.*bx(2,j));
	real y0  =w(iw(5)-1+j-1)+ds*(by(0,j)+ds*(by(1,j)+ds*by(2,j)));
	real y0s =by(0,j)+ds*(2.*by(1,j)+ds*3.*by(2,j));
	real y0ss=2.*(by(1,j)+ds*3.*by(2,j));

	//.......stretching in s direction (near corners etc.)
	real s0,s0s;
	STRT( r(i,axis1),s0,s0s, iwk(4+3*ndi),rwk(3*ndr),ierr ); // STRT( s,s0,s0s,   iws(0),rws(0),ierr ); 
      
	if( computeMap )
	{
	  x(i,axis1)=x0;
	  x(i,axis2)=y0;
          if( rangeDimension==3 )
            x(i,axis3)=zLevel;
	}
      
	if( computeMapDerivative )
	{
	  xr(i,axis1,axis1)=x0s;
	  xr(i,axis2,axis1)=y0s;
          if( rangeDimension==3 )
            xr(i,axis3,axis1)=0.;
	}    
      }
    }
  }
  

}

void SmoothedPolygon::
mapS( const RealArray & r_, RealArray & x, RealArray & xr, MappingParameters & params )
// ===========================================================================================
// /Description:
//   Evaluate the smoothed polygon mapping -- serial array version --
// ===========================================================================================
{
  if( params.coordinateType != cartesian )
    cerr << "SmoothedPolygon::map - coordinateType != cartesian " << endl;

  if( !mappingIsDefined)
     initialize();

//   #ifdef USE_PPP
//     MPI_Barrier(Overture::OV_COMM);  // Add this for testing
//   #endif

  Index I = getIndex( r_,x,xr,base,bound,computeMap,computeMapDerivative );

  int ierr;
  const real BIG_REAL = REAL_MAX*.1;
  
  Range D(0,domainDimension-1);
  realSerialArray r(I,D);
  r=r_(I,D);
  if( getIsPeriodic(axis1)==functionPeriodic )
  {
    r(I,axis1)=fmod(r(I,axis1)+1.,1.);  // map back to [0,1]
  }
  

  const real rMax=.5+2.;   // only evaluate for r in [-2,2]
  const real signOfNormalDistance = normalDistance>0. ? 1 : -1.;
  
  if( domainDimension==2 )
  {

    for( int i=base; i<=bound; i++ )
    {
      if( fabs(r(i,axis1)-.5)>rMax )
      {
	if( computeMap )
	{
	  x(i,axis1)=x(i,axis2)=BIG_REAL;
          if( rangeDimension==3 )
	    x(i,axis3)=BIG_REAL;
	}
	if( computeMapDerivative )
	{
	  xr(i,axis1,axis1)=xr(i,axis2,axis2)=1.;
	  xr(i,axis1,axis2)=xr(i,axis2,axis1)=0.;
          if( rangeDimension==3 )
            xr(i,axis3,Range(0,1))=0.;
	}
      }
      else
      {
	// real s=r(i,axis1);
	int j= int(r(i,axis1)*nsm1+1);
	j=max(min(j,nsm1),1);

	real ds=r(i,axis1)-(j-1)/real(nsm1);

        // evaluate the spline
	real x0  =w(iw(4)-1+j-1)+ds*(bx(0,j)+ds*(bx(1,j)+ds*bx(2,j)));
	real x0s =bx(0,j)+ds*(2.*bx(1,j)+ds*3.*bx(2,j));
	real x0ss=2.*(bx(1,j)+ds*3.*bx(2,j));
	real y0  =w(iw(5)-1+j-1)+ds*(by(0,j)+ds*(by(1,j)+ds*by(2,j)));
	real y0s =by(0,j)+ds*(2.*by(1,j)+ds*3.*by(2,j));
	real y0ss=2.*(by(1,j)+ds*3.*by(2,j));

	

	//.......stretching in s direction (near corners etc.)
	real s0,s0s;
	STRT( r(i,axis1),s0,s0s, iwk(4+3*ndi),rwk(3*ndr),ierr ); // STRT( s,s0,s0s,   iws(0),rws(0),ierr ); 
      
	//.......radial width varies in s direction
	real r0,r0s0;
	STTR( s0,r0,r0s0, iwk(4+2*ndi),rwk(2*ndr),ierr );  //    STTR( s0,r0,r0s0, iwr(0),rwr(0),ierr );
      
	//........lines stretched in r direction
	real r1,r1r;
	STRT(r(i,axis2),r1,r1r,iwk(4+4*ndi),rwk(4*ndr),ierr); //STRT(r(i,axis2),r1,r1r,iwr1(0),rwr1(0),ierr );
      
	real rr0=r1*r0;
	real d=1./SQRT(x0s*x0s+y0s*y0s);
      
        //  Usually the grid is defined by moving in the normal direction from the boundary
        //    x(s,r) = x0(s) + r*d0(s)
        //            d0(s) = (-y0s(s),x0s(s))/|(x0s,y0s)|) * D0
        //  Near corners we may adjust the direction to match the normal to the polygon:
        //
        //    The correction vector is
        //          dc = [  (-yvs,xvs)/|(xvs,yvs)|  - (-y0s(0),x0s(0))/|(x0s(0),y0s(0))| ]
        //      d(s) = d0(s) + dc* D0 * (1-alpha(s)
        // 

        real dx = -rr0*y0s*d;  // direction vector is (dx,dy)
	real dy =  rr0*x0s*d;

	real rr0s=r1*r0s0*s0s;
	real tmp=-(x0s*y0ss-y0s*x0ss)*d*d*d;

	real dxs = rr0*x0s*tmp-rr0s*y0s*d;      // (dxs,dys) : derivative w.r.t. tangential variable s
	real dys = rr0*y0s*tmp+rr0s*x0s*d;
	real dxr = -r1r*r0*y0s*d;               // (dxr,dyr) : derivative w.r.t. the normal variable r
	real dyr = +r1r*r0*x0s*d;

        bool adjustDirection=ccor(18) > 0.5;
        const real rw=.1;  // width of transition
        if( adjustDirection && fabs(r(i,axis1)-.5)>(.5-rw) )
	{
          // Adjust the tangents to match the slope of the final polygon line segment
          int je; 
          real xvs,yvs,xs0,ys0,alpha,alphas,scale,dvs,dcx,dcy;

          // slope of last segment of the polygon is (xvs,yvs)
          if( r(i,axis1)<.5 )
          {
            je=1;  // end point of spline

            // use specified end vector
	    yvs =-(ccor(12)-ccor(10));
	    xvs = (ccor(16)-ccor(14));
            // printF("Start: (xvs,yvs)=(%8.4f,%8.4f) ",xvs,yvs);

	    // xvs = vertex(1,0)-vertex(0,0);   
	    // yvs = vertex(1,1)-vertex(0,1);       
            // printF("old: (xvs,yvs)=(%8.4f,%8.4f) \n",xvs,yvs);

            dvs = r(i,axis1)/rw;
	    // alpha = max(0,r(i,axis1)/rw);
	    // blending function f(x) = x + x^2 - x^3  
	    //                   f'(x)= 1 + 2x - 3x^2
	    //  f(0)=0, f'(0)=1 f(1)=1, f'(1)=0
            // s = r(i,0)
	    alpha= dvs*( 1.+dvs*(1.-dvs) );  
	    alphas= (1.+dvs*(2.-3.*dvs))/rw;
	  }
	  else
	  {
            je=nsm1+1;

            // use specified end vector
	    yvs =-(ccor(13)-ccor(11));
	    xvs = (ccor(17)-ccor(15));
            // printF("End: (xvs,yvs)=(%8.4f,%8.4f) ",xvs,yvs);

	    // xvs = vertex(numberOfVertices-1,0)-vertex(numberOfVertices-2,0);   
	    // yvs = vertex(numberOfVertices-1,1)-vertex(numberOfVertices-2,1);       

            // printF("old: (xvs,yvs)=(%8.4f,%8.4f) \n",xvs,yvs);

            dvs=(1.-r(i,axis1))/rw;
	    alpha= dvs*( 1.+dvs*(1.-dvs) );  
	    alphas= -(1.+dvs*(2.-3.*dvs))/rw;
	  }
          scale=sqrt(xvs*xvs+yvs*yvs);
          xvs/=scale; yvs/=scale;         // normalize direction to the polygon
	  
          // slope of spline at end is (xs0,ys0)
	  xs0 = bx(0,je); // derivative on end
	  ys0 = by(0,je); 
          scale=sqrt(xs0*xs0+ys0*ys0)*signOfNormalDistance;
	  xs0/=scale; ys0/=scale;            // normalize

          dcx = -(yvs - ys0); // here is the (constant) correction vector for direction at the end
          dcy =  (xvs - xs0);

          dx += rr0*dcx*(1.-alpha);   //  add correction to direction 
	  dy += rr0*dcy*(1.-alpha);
	  
          dxs += rr0s*dcx*(1.-alpha) -alphas*rr0*dcx;   // adjust derivatives
          dys += rr0s*dcy*(1.-alpha) -alphas*rr0*dcy;

          dxr += r1r*r0*dcx*(1.-alpha);
          dyr += r1r*r0*dcy*(1.-alpha);
	}

	if( computeMap )
	{
	  // x(i,axis1)=x0-rr0*y0s*d;
	  // x(i,axis2)=y0+rr0*x0s*d;
	  x(i,axis1)=x0+dx;
	  x(i,axis2)=y0+dy;
          if( rangeDimension==3 )
  	    x(i,axis3)=zLevel;
	}
      
	if( computeMapDerivative )
	{
	  // real rr0s=r1*r0s0*s0s;
	  // real tmp=-(x0s*y0ss-y0s*x0ss)*d*d*d;
	  // xr(i,axis1,axis1)=x0s+rr0*x0s*tmp-rr0s*y0s*d;
	  // xr(i,axis2,axis1)=y0s+rr0*y0s*tmp+rr0s*x0s*d;
	  // xr(i,axis1,axis2)=-r1r*r0*y0s*d;
	  // xr(i,axis2,axis2)=+r1r*r0*x0s*d;

	  xr(i,axis1,axis1)=x0s+dxs;
	  xr(i,axis2,axis1)=y0s+dys;
	  xr(i,axis1,axis2)=    dxr;
	  xr(i,axis2,axis2)=    dyr;

          if( rangeDimension==3 )
            xr(i,axis3,Range(0,1))=0.;
	}    

	//.....Now project onto (straight) boundary curves s=0 and s=1 *** is this still needed?

	if( ccor(18) > 0.5 )
	{
	  real epss=ccor(9);
	  // cout << "SP: epss = " << epss << endl;
	  if( fabs(r(i,0))<epss )
	  {
	    if( computeMap )
	    {
	      x(i,0)=ccor(10)+r1*(ccor(12)-ccor(10));
	      x(i,1)=ccor(14)+r1*(ccor(16)-ccor(14));
	    }
	  
	    if( computeMapDerivative )
	    {
	      xr(i,0,1)=(ccor(12)-ccor(10))*r1r;
	      xr(i,1,1)=(ccor(16)-ccor(14))*r1r;
	    }
	  }
	  else if( fabs(r(i,0)-1.)<epss )
	  {
	    if( computeMap )
	    {
	      x(i,0)=ccor(11)+r1*(ccor(13)-ccor(11));
	      x(i,1)=ccor(15)+r1*(ccor(17)-ccor(15));
	    }
	  
	    if( computeMapDerivative )
	    {
	      xr(i,0,1)=(ccor(13)-ccor(11))*r1r;
	      xr(i,1,1)=(ccor(17)-ccor(15))*r1r;
	    }
	  }
	}
      }
    }
  }
  else 
  { // domain dimension==1 : polygon is just a curve

    for( int i=base; i<=bound; i++ )
    {
      if( fabs(r(i,axis1)-.5)>rMax )
      {
	if( computeMap )
	{
	  x(i,axis1)=x(i,axis2)=BIG_REAL;
          if( rangeDimension==3 )
            x(i,axis3)=BIG_REAL;
	}
	if( computeMapDerivative )
	{
	  xr(i,axis1,axis1)=1.;
	  xr(i,axis2,axis1)=0.;
          if( rangeDimension==3 )
            xr(i,axis3,axis1)=0.;
	}
      }
      else
      {
	// real s=r(i,axis1);
	int j= int(r(i,axis1)*nsm1+1);
	j=max(min(j,nsm1),1);

	real ds=r(i,axis1)-(j-1)/real(nsm1);

	real x0  =w(iw(4)-1+j-1)+ds*(bx(0,j)+ds*(bx(1,j)+ds*bx(2,j)));
	real x0s =bx(0,j)+ds*(2.*bx(1,j)+ds*3.*bx(2,j));
	real x0ss=2.*(bx(1,j)+ds*3.*bx(2,j));
	real y0  =w(iw(5)-1+j-1)+ds*(by(0,j)+ds*(by(1,j)+ds*by(2,j)));
	real y0s =by(0,j)+ds*(2.*by(1,j)+ds*3.*by(2,j));
	real y0ss=2.*(by(1,j)+ds*3.*by(2,j));

	//.......stretching in s direction (near corners etc.)
	real s0,s0s;
	STRT( r(i,axis1),s0,s0s, iwk(4+3*ndi),rwk(3*ndr),ierr ); // STRT( s,s0,s0s,   iws(0),rws(0),ierr ); 
      
	if( computeMap )
	{
	  x(i,axis1)=x0;
	  x(i,axis2)=y0;
          if( rangeDimension==3 )
            x(i,axis3)=zLevel;
	}
      
	if( computeMapDerivative )
	{
	  xr(i,axis1,axis1)=x0s;
	  xr(i,axis2,axis1)=y0s;
          if( rangeDimension==3 )
            xr(i,axis3,axis1)=0.;
	}    
      }
    }
  }
  

}


//=================================================================================
// get a mapping from the database
//=================================================================================
int SmoothedPolygon::
get( const GenericDataBase & dir, const aString & name)
{
  if( debug & 4 )
    printF("Entering SmoothedPolygon::get\n");

  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");

  subDir.get( SmoothedPolygon::className,"className" ); 
  if( SmoothedPolygon::className != "SmoothedPolygon" )
  {
    printF("SmoothedPolygon::get ERROR in className!\n");
  }
  subDir.get(mappingIsDefined,"mappingIsDefined" );
  subDir.get(numberOfVertices,"numberOfVertices");
  subDir.get(splineResolutionFactor,"splineResolutionFactor");
  vertex.redim(0);
  subDir.get(vertex          ,"vertex");
  arclength.redim(0);
  subDir.get(arclength       ,"arclength");
  subDir.get(NumberOfRStretch,"NumberOfRStretch");
  subDir.get(userStretchedInT,"userStretchedInT");
  subDir.get(correctCorners  ,"correctCorners");
  bv.redim(0);
  subDir.get(bv              ,"bv");
  sab.redim(0);
  subDir.get(sab             ,"sab");
  r12b.redim(0);
  subDir.get(r12b            ,"r12b");
  sr.redim(0);
  subDir.get(sr              ,"sr");
  corner.redim(0);
  subDir.get(corner          ,"corner");
  subDir.get(numberOfRStretch,"numberOfRStretch");
  subDir.get(normalDistance  ,"normalDistance");
  subDir.get(zLevel  ,"zLevel");
  subDir.get( ndiwk,"ndiwk" );
  subDir.get( ndrwk,"ndrwk" );
  iwk.redim(0);
  subDir.get( iwk,"iwk" );
  rwk.redim(0);
  subDir.get( rwk,"rwk" );
  Mapping::get( subDir, "Mapping" );

  // fix up adoptions
  if( mappingIsDefined )
    assignAdoptions();

  delete &subDir;
  mappingHasChanged();
  return 0; 
}



int SmoothedPolygon::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put(SmoothedPolygon::className,"className" );
  subDir.put(mappingIsDefined,"mappingIsDefined" );
  subDir.put(numberOfVertices,"numberOfVertices");
  subDir.put(splineResolutionFactor,"splineResolutionFactor");
  subDir.put(vertex          ,"vertex");
  subDir.put(arclength       ,"arclength");
  subDir.put(NumberOfRStretch,"NumberOfRStretch");
  subDir.put(userStretchedInT,"userStretchedInT");
  subDir.put(correctCorners  ,"correctCorners");
  subDir.put(bv              ,"bv");
  subDir.put(sab             ,"sab");
  subDir.put(r12b            ,"r12b");
  subDir.put(sr              ,"sr");
  subDir.put(corner          ,"corner");
  subDir.put(numberOfRStretch,"numberOfRStretch");
  subDir.put(normalDistance  ,"normalDistance");
  subDir.put(zLevel  ,"zLevel");
  subDir.put(ndiwk,"ndiwk" );
  subDir.put(ndrwk,"ndrwk" );
  subDir.put(iwk,"iwk" );
  subDir.put(rwk,"rwk" );
  Mapping::put(subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping *SmoothedPolygon::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=NULL;
  if( mappingClassName==SmoothedPolygon::className )
    retval = new SmoothedPolygon();
  return retval;
}

int SmoothedPolygon::
update( MappingInformation & mapInfo ) 
//===========================================================================
// /Purpose: 
//   Prompt for parameters defining the smooth polygon. 
//===========================================================================
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sPrintF
  aString menu[] = 
    {
      "!SmoothedPolygonMapping",
      "plot",
      "vertices",
      "sharpness",
      "t-stretch",
      "n-stretch",
      "correct corners",
      "do not correct corners",
      "corners",
      "n-dist",
      "curve or area (toggle)",
      "make 3d (toggle)",
      "set resolution for approximating spline",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "check",
      "check inverse",
      "check derivative",
      "use robust inverse",
      "do not use robust inverse",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "plot      : plot the grid",
      "vertices  : Specify new vertices for the polygon",
      "lines     : Enter number of grid lines in the n and t directions",
      "sharpness : Specify how sharp the corners are (exponent)",
      "t-stretch : Specify stretching in tangent-direction",
      "n-stretch : Specify stretching in normal-direction",
      "correct corners : auto correct corners to match normal to true polygon",
      "corners   : Fix the grid vertices to specific positions",
      "n-dist    : Specify normal distance at vertex(+-epsilon)",
      "curve or area (toggle) : make mapping into a curve or an area",
      "make 3d (toggle)   : make a 2D grid a surface in 3d or vice versa",
      "set resolution for approximating spline : the smooth polygon is approximated by a spline",
      " ",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "check inverse      : input points to check the inverse",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help      : Print this list",
      "exit      : Finished with parameters, construct grid",
      "" 
    };

  aString answer,line; 

  bool plotObject=TRUE;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.outputString( ">>>Grid Defined from a Smoothed Polygon<<<" );
  gi.outputString( ">>>r(1) = tangent direction, r(2) = normal direction<<<" );
  gi.outputString( ">>>Positive normal distance follows the left-hand rule<<<" );

  gi.appendToTheDefaultPrompt("SmoothedPolygon>"); // set the default prompt

  int i;
  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="vertices" )   // specify new vertices
    {

      if( !mappingIsDefined )
        numberOfVertices=4;  // default value
      else
      {
        gi.inputString(line,"Enter the number of vertices on the polygon: ");  // *** check this ***
        if( line!="" ) sScanF( line,"%i",&numberOfVertices );
      }
      vertex.redim(numberOfVertices,2); vertex=0.;
      arclength.redim(numberOfVertices); arclength=0.;
      real ds=0.,dsmax=0.,dsmin=0.;

      if( getGridDimensions(0)<=0 || getGridDimensions(1)<=0 )
      {
        setGridDimensions(0,7*numberOfVertices);   // number of grid lines in tangential
        setGridDimensions(1,7);                    // number of grid lines in normal direction
      }
      if( !mappingIsDefined )
      { // default points are (0,0) (1,0)  (1,1) (0,1)
        vertex=0;  vertex(1,0)=1.; vertex(2,0)=1.; vertex(2,1)=1.; vertex(3,1)=1.;
        dsmax=dsmin=1.;
      } 
      else
      {
	for( i=0; i<numberOfVertices; i++ )
	{
	  gi.inputString(line,sPrintF(buff,"Enter (x,y) for point %i: ",i));
	  sScanF( line,"%f %f",&vertex(i,0),&vertex(i,1) );
	  if( i>0 )
	  {
	    ds=SQRT( SQR(vertex(i,0)-vertex(i-1,0)) + SQR(vertex(i,1)-vertex(i-1,1)) );
	    dsmax= i>1 ? max(dsmax,ds) : ds;
	    dsmin= i>1 ? min(dsmin,ds) : ds;
	  }
	}
      }
      //  ...assign default values for parameters
      numberOfRStretch=1;
      sr.redim(3,numberOfRStretch);   sr=0.;
      bv.redim(numberOfVertices);     bv=0.;
      sab.redim(2,numberOfVertices);  sab=0.;
      r12b.redim(3,numberOfVertices); r12b=0.;

      normalDistance=-(dsmax+dsmin)*.05;  // guess normal length to use
      for( i=0; i<numberOfVertices; i++ )
      {
	bv(i)=40.    ;               // sharpness of corners
	sab(0,i)=.15 ;               // weight : stretching in t direction at vertices
	sab(1,i)=50. ;               // exponent : stretching in t-direction ..
	r12b(0,i)=normalDistance ;   // radius at point i+
	r12b(1,i)=normalDistance ;   // radius at point (i+1)-
	r12b(2,i)=50.;               // transition factor from ? to ?
      }
      
      // if the polygon is not closed, reset periodicity if set
      if( SQR(vertex(numberOfVertices-1,0)-vertex(0,0)) 
        + SQR(vertex(numberOfVertices-1,1)-vertex(0,1)) > 10.*REAL_EPSILON )
      {
	if( getIsPeriodic(axis1) != Mapping::notPeriodic )
        {
          gi.outputString("reseting periodicity and boundary conditions to not periodic along axis1");
          setIsPeriodic(axis1,Mapping::notPeriodic);
          setBoundaryCondition(Start,axis1,1);
          setBoundaryCondition(End,  axis1,1);
	}
      }
      userStretchedInT=FALSE;        // becomes true when user specifies t-stretch
      sab(0,0) =0.;                  // no stretching at end points by default
      sab(0,numberOfVertices-1)=0.;

      if( !userStretchedInT ) // if user has not specified stretching in the t-direction
      {
	if( getIsPeriodic(axis1)==functionPeriodic )  // set t-stretching at one periodic end
	  sab(0,0) =.15;
	else
	  sab(0,0) =.0;                            // no t-stretching at end by default
      }	
      
      sr(0,0)=1.;           // default stretching in the r-direction
      sr(1,0)=4.;
      sr(2,0)=0.;

      correctCorners=FALSE;  // correct corners? 0=no
      corner.redim(2,2,2);
      corner=0.;  

      mappingHasChanged();
      mappingIsDefined=TRUE;   // the smoothed polygon is now defined
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity"  ||
             answer=="check"   || 
             answer=="check inverse"   || 
             answer=="check derivative"   || 
             answer=="use robust inverse" || 
             answer=="do not use robust inverse" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;

      if( !userStretchedInT ) // if user has not specified stretching in the t-direction
      {
	if( getIsPeriodic(axis1)==functionPeriodic )  // set t-stretching at one periodic end
	  sab(0,0) =.15;
	else
	  sab(0,0) =.0;                            // no t-stretching at end by default
      }	
      if( answer=="lines" )
      {
        // rebuild the approximating spline
        mappingHasChanged();
      }
      if( answer=="mappingName" )
        continue;
    }
    else if( answer=="sharpness" )
    {
      for( i=0; i<numberOfVertices; i++ )
      {
        gi.inputString(line,sPrintF(buff,"Enter exponent b for point %i (default=%7.2e)",i,bv(i)));
        if( line!="" ) sScanF( line,"%e",&bv(i) );
        printF(" bv(%i) = %g\n",i,bv(i));
      }
      mappingHasChanged();
    }
    else if( answer=="t-stretch")
    {
      userStretchedInT=TRUE;  // becomes true when user specifies t-stretch
      gi.outputString( "Stretch lines in t-direction at vertices..." );
      for( i=0; i<numberOfVertices; i++ )
      {
        gi.inputString(line,sPrintF(buff,"Enter a,b (weight,exponent) for "
          "point %i (default=%7.2e,%7.2e)",i,sab(0,i), sab(1,i)));
        if( line!="" ) sScanF( line,"%e %e",&sab(0,i),&sab(1,i) );
      }
      if( getIsPeriodic(axis1)==functionPeriodic )  // set t-stretching at one periodic end
        sab(0,numberOfVertices-1) =0.;  // if periodic this is zero
      mappingHasChanged();
    }
    else if( answer=="n-stretch")
    {
      gi.outputString( "Stretch lines in n-direction at vertices..." );
      gi.inputString(line,sPrintF(buff,"Enter a,b,c (weight,exponent,position) for "
          "normal stretching (default=%7.2e,%7.2e,%7.2e)",sr(0,0),sr(1,0),sr(2,0)));
      if( line!="" ) sScanF( line,"%e %e %e",&sr(0,0),&sr(1,0),&sr(2,0) );
      mappingHasChanged();
    }
    else if( answer=="curve or area (toggle)" )
    {
      if( domainDimension==2 )
	setDomainDimension(1);
      else
	setDomainDimension(2);
      mappingHasChanged();
    }
    else if( answer=="do not correct corners")
    {
      correctCorners=false;
    }
    else if( answer=="correct corners")
    {
      printF("INFO: automatically correct corners to match the polygon. This will cause the `normals' \n"
             "      on the end points to match the normals to the polygon.\n");
      
      if( correctCorners )
      {
	printF(" Corners have already been corrected! No changes made\n");
	continue;
      }

      correctPolygonForCorners();  // *wdh* 100305 

//       corner(axis1,0,0)=vertex(0,0);
//       corner(axis2,0,0)=vertex(0,1);
//       corner(axis1,1,0)=vertex(numberOfVertices-1,0);
//       corner(axis2,1,0)=vertex(numberOfVertices-1,1);

//       const real signOfNormalDistance = normalDistance>0. ? 1 : -1.;
      
//       // get normals to the polygon:
//       int ie=0;  // start pt index
//       real nx1 = - (vertex(ie+1,1)-vertex(ie,1));
//       real ny1 =   (vertex(ie+1,0)-vertex(ie,0));
//       real norm= sqrt( nx1*nx1+ny1*ny1 )*signOfNormalDistance;
//       nx1/=norm; ny1/=norm;
      
//       ie=numberOfVertices-1;
//       real nx2 = - (vertex(ie,1)-vertex(ie-1,1));
//       real ny2 =   (vertex(ie,0)-vertex(ie-1,0));
//       norm= sqrt( nx2*nx2+ny2*ny2 )*signOfNormalDistance;
//       nx2/=norm; ny2/=norm;
      
//       // evaluate the mapping to get the normal distance
//       realArray r(4,2),x(4,2);	
//       r(0,0)=0.; r(0,1)=0.; 
//       r(1,0)=0.; r(1,1)=1.; 
//       r(2,0)=1.; r(2,1)=0.; 
//       r(3,0)=1.; r(3,1)=1.; 
//       map(r,x);

//       real nDist1 = sqrt( SQR(x(1,0)-x(0,0))+SQR(x(1,1)-x(0,1)) );
//       real nDist2 = sqrt( SQR(x(3,0)-x(2,0))+SQR(x(3,1)-x(2,1)) );
	
//       corner(axis1,0,1)=corner(axis1,0,0)+nx1*nDist1;
//       corner(axis2,0,1)=corner(axis2,0,0)+ny1*nDist1;

//       corner(axis1,1,1)=corner(axis1,1,0)+nx2*nDist2;
//       corner(axis2,1,1)=corner(axis2,1,0)+ny2*nDist2;

//       correctCorners=true;
//       // ::display(corner,"corner (new)");

//       mappingHasChanged();

    }
    else if( answer=="corners")
    {
      printF("INFO: to automatically correct corners use the new option `correct corners'\n");
      aString cornerMenu[] = {"specify positions of corners",
                             "do not specify positions of the corners",
                             "" };  

      gi.getMenuItem(cornerMenu,answer);
      if( answer=="do not specify positions of the corners")
        correctCorners=false;
      else
      {
        // default positions:
        corner(axis1,0,0)=vertex(0,0);
        corner(axis2,0,0)=vertex(0,1);
        corner(axis1,1,0)=vertex(numberOfVertices-1,0);
        corner(axis2,1,0)=vertex(numberOfVertices-1,1);

        correctCorners=false;
        // evaluate the mapping to get positions of outer corners
        realArray r(4,2),x(4,2);	
        r(0,0)=0.; r(0,1)=0.; 
        r(1,0)=0.; r(1,1)=1.; 
        r(2,0)=1.; r(2,1)=0.; 
        r(3,0)=1.; r(3,1)=1.; 
	map(r,x);

        corner(axis1,0,1)=corner(axis1,0,0)+(x(1,0)-x(0,0));
        corner(axis2,0,1)=corner(axis2,0,0)+(x(1,1)-x(0,1));

        corner(axis1,1,1)=corner(axis1,1,0)+(x(3,0)-x(2,0));
        corner(axis2,1,1)=corner(axis2,1,0)+(x(3,1)-x(2,1));


	gi.outputString( "Fix the positions of the 4 corners of the grid" );
	for( int i2=0; i2<=1; i2++ )
	  for( int i1=0; i1<=1; i1++ )
	  {
	    gi.inputString(line,sPrintF(buff,"Enter vertex (%i,%i)"
					"default=( %7.2e,%7.2e)",i1,i2,corner(0,i1,i2),corner(1,i1,i2)));
	    if( line!="" ) sScanF( line,"%e %e",&corner(0,i1,i2),&corner(1,i1,i2) );
	  }
      }
      // ::display(corner,"corner (old)");
      
      correctCorners=true;
      mappingHasChanged();
    }
    else if( answer=="n-dist" )
    {
      aString normalMenu[] = {"fixed normal distance",
                             "variable normal distance",
                             "" };  
      gi.getMenuItem(normalMenu,answer);
      gi.outputString( "Normal distance may be positive or negative" );
      if( answer=="fixed normal distance" )
      {
        gi.inputString(line,sPrintF(buff,"Enter the normal distance (default=%e)",normalDistance));
        if( line!="" ) sScanF( line,"%e",&normalDistance );
        for( i=0; i<numberOfVertices; i++ )
        {
          r12b(0,i)=normalDistance;  // radius at point i+
          r12b(1,i)=normalDistance;  // radius at point (i+1)-
          r12b(2,i)=50. ;  // transition factor from ? to ?
	}
      }
      else if( answer=="variable normal distance" )
      {
        gi.outputString( "Normal distance can vary from vertex to vertex" );
        gi.outputString( "dn(i)+: normal distance at (vertex i)+epsilon" );
        gi.outputString( "dn(i)-: normal distance at (vertex i)-epsilon" );
        gi.outputString( "b(i) : exponent for transition at vertex i+1" );
        for( i=0; i<numberOfVertices-1; i++ )
        {
          gi.inputString(line,sPrintF(buff,"Enter dn(%i)+,dn(%i)- b(%i)  (default="
			   " %e,%e,%e): ",i,i+1,i,r12b(0,i),r12b(1,i),r12b(2,i)));
          if( line!="" ) sScanF( line,"%e %e %e",&r12b(0,i),&r12b(1,i),&r12b(2,i) );
	}
      }
      else
      {
	printF("Unknown response =[%s], try again.\n",(const char*)answer);
	gi.stopReadingCommandFile();
      }
      mappingHasChanged();
    }
/* ---
    else if( answer=="isPeriodic" )
    {
      aString periodicMenu[] = {"polygon is periodic",
                               "derivative of the polygon is periodic",
                               "polygon is not periodic",
                               "" };  
      gi.getMenuItem(periodicMenu,answer);
      if( answer=="polygon is periodic" )
        setIsPeriodic(axis1,functionPeriodic);
      else if(answer=="derivative of the polygon is periodic")
        setIsPeriodic(axis1,derivativePeriodic);
      else if(answer=="polygon is not periodic")
        setIsPeriodic(axis1,notPeriodic);
      else
        cout << "unknown resposnse! answer =" << answer << endl;

      if( !userStretchedInT ) // if user has not specified stretching in the t-direction
      {
	if( isPeriodic(axis1)==functionPeriodic )  // set t-stretching at one periodic end
	  sab(0,0) =.15;
	else
	  sab(0,0) =.0;                            // no t-stretching at end by default
      }	
    }
----- */
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
    else if( answer=="set resolution for approximating spline" )
    {
      printF(" The smoothed polygon curve is approximated by a spline with \n"
             " number of spline knots equal to numberOfGridPoints along axis=0 times the splineResolutionFactor\n"
             " Choose a larger value for the splineResolutionFactor to get a more accurate approximation");
      
      gi.inputString(line,sPrintF(buff,"Enter the splineResolutionFactor (default=%f): ",splineResolutionFactor));
      if( line!="" )
      {
        sScanF( line,"%e",&splineResolutionFactor);
        if( splineResolutionFactor<0. )
          splineResolutionFactor=5.;
	
        printF(" New splineResolutionFactor= %f\n",splineResolutionFactor);
      }
    }
    else if( answer=="show parameters" )   // display values of parameters
    {
      printF(" vertex                    corner         normal distance          t-stretch \n");
      printF("   i       x        y     sharpness    dn(i)+  dn(i+1)-   db       a      b     \n");
      printF(" ------  -----    -----   ---------   -------  -------  ------   ------ ------   \n");
      for( i=0; i<numberOfVertices; i++ )
      {
        printF("%5i  %6.3e %6.3e  %5.3f  %6.3e %6.3e %5.3f  %5.3f %5.3f \n",i,vertex(i,0),
         vertex(i,1),bv(i),r12b(0,i),r12b(1,i),r12b(2,i), sab(0,i),sab(1,i));
      }
      printF("dn(i)+: normal distance at (vertex i)+epsilon \n");
      printF("dn(i)-: normal distance at (vertex i)-epsilon\n");
      printF("db(i) : exponent for transition from dn(i)+ to dn(i+1)-1 \n");
      printF("t-stretch: stretching in tangential direction at corner is: a(i)*tanh(b(i)*(t-corner(i))\n");
      printF("Radial stretching is: a=%f, b=%f, c=%f\n",sr(0,0),sr(1,0),sr(2,0));
      printF("Corners adjusted to fixed positions = %s\n",(correctCorners==TRUE ? "TRUE" : "FALSE"));
      display();
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
      for( i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
      continue;
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
      //     ---allocate work-spaces
      //      (ndi,ndr) : used by cpinit to call stretching routines
      //         ndi >= 12 if not periodic
      //         ndi >= 12+(nu+nv)*2 if periodic
      //        ndr >=3*(nu+nv+1)+4+nv+nsp*4+extra if periodic
      //           Usually nu+nv <= extra <= 2*(nu+nv)


      initialize();
/* -----      
      int numberOfSplinePoints=getGridDimensions(axis1)*2; // number of points for spline in t direction

      int ndi=12+(numberOfVertices)*2;
      int ndr=3*(numberOfVertices+1)+4+0+numberOfSplinePoints*5+2*(numberOfVertices)+100;

      int ndwsp=9+9*numberOfSplinePoints; // work space for splines in cprsi

      const int ndccor=20;  // ???
      
      ndiwk=5*ndi+4;
      ndrwk=5*ndr+ndccor+ndwsp;

      iwk.redim(ndiwk);
      rwk.redim(ndrwk);
      
      // ...........Initialization call for CPR

      int ndwk=100;
      RealArray wk(ndwk);
      
      iwk(0)=ndi;
      iwk(1)=ndr;
      CPINIT( ndi,iwk(4),iwk(4+ndi),iwk(4+2*ndi),
	     iwk(4+3*ndi),iwk(4+4*ndi),
	     ndr,rwk(0),rwk(ndr),rwk(2*ndr),
	     rwk(3*ndr),rwk(4*ndr),
	     rwk(5*ndr),
	     numberOfVertices,arclength(0),vertex(0,0),vertex(0,1),bv(0),sab(0,0),r12b(0,0), 
	     numberOfRStretch,sr(0,0), ndwk,wk(0),
	     vertex(0,0,0),vertex(0,1,0),vertex(0,0,1),vertex(0,1,1),
	     vertex(1,0,0),vertex(1,1,0),vertex(1,0,1),vertex(1,1,1),
	     correctCorners,isPeriodic(axis1) );
      //.....Initialize spline routines cprs

      iwk(2)=ndwsp;
      iwk(3)=1+5*ndr+ndccor;
      CPRSI( numberOfSplinePoints,numberOfSplinePoints,isPeriodic(axis1), ndwsp,
	    rwk(-1+iwk(3)),rwk(-1+iwk(3)),
	    ndi,iwk(4),iwk(4+ndi),iwk(4+2*ndi),
	    iwk(4+3*ndi),iwk(4+4*ndi),
	    ndr,rwk(0),rwk(ndr),rwk(2*ndr),
	    rwk(3*ndr),rwk(4*ndr),
	    rwk(5*ndr) );
------- */

      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***

    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}

