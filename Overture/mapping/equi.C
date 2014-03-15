#include "EquiDistribute.h"
#include "arrayGetIndex.h"
#include "display.h"

#ifdef USE_PPP
int
equidistribute( const realArray & w, realArray & r )
{
  Overture::abort("equidistribute: finish me for parallel.");
}
#endif



int
equidistribute( const RealArray & w, RealArray & r )
// =====================================================================
/// 
/// \details 
///     Equi-distribute the function w
/// 
/// \param w (input) : weight function with positive entries
/// \param r (output) : 0 <= r(i) <= 1 : strictly increasing values satisfying
///      \int_0^{r_i} w(r) dr = (i/N) 
/// 
// =====================================================================
{
  int base=w.getBase(0), bound=w.getBound(0);
  Range R(base,bound);
  
  // fi = integral of f = .5*f_0 + f_1 + f_2 + ... + .5*f_N
  real fi=.5*(w(base)+w(bound));
  fi+=sum(w(Range(base+1,bound-1)));
  // printf(" fi = %f \n",fi);
  
    
  // s0 = integral to j-1
  // s1 = integral to j
  int j=base;
  real s0=0.;
  real s1=.5*(w(j)+w(j+1));
  j++;
  const real eps=.1*sqrt(REAL_EPSILON);

  real h=1./(bound-base);
  r(base)=0.;
  for( int i=base+1; i<=bound-1; i++ )
  {
    real sBar=(i-base)*fi*h;   // find r(i) so integral_0^{r_i} f dr = sBar
    
    while( s1<sBar )
    {
      s0=s1;
      s1+=.5*(w(j)+w(j+1));
      j++;
    }
    // There was a bug on the sun's when this file was compiled with -O (mastSail2d.cmd)
    // this bug went away when I added the asserts and the print statement below. *wdh* 991113

    assert( j<=bound );
    // s(delta) = s0 + .5*( w(j-1)+delta*(w(j)-w(j-1)) )*delta  ,   0<= delta <= 1
    // solving for s(delta)=sBar gives delta:
    real delta;
    real a = (sBar-s0)*(w(j)-w(j-1))/(w(j-1)*w(j-1));
    if( fabs(a) < eps ) // *****
      delta=  (sBar-s0)/w(j-1);
    else
    {
      assert( a>= -.5 );
      delta =w(j-1)*( -1.+sqrt(1.+2.*a) )/(w(j)-w(j-1));
    }
    
    r(i)=(j-1+delta)*h;

    if( r(i) <= r(i-1) )
    {
      printf("equidistribute:ERROR: r(%i)=%e r(%i)=%e w(%i)=%e w(%i)=%e \n",i-1,r(i-1),i,r(i),j-1,w(j-1),j,w(j));
      {throw "error";}
    }
  }
  r(bound)=1.;

  return 0;
}

int
periodicUpdate( realArray & x, const 
		IntegerArray & indexRange, 
		const IntegerArray & bc,
                const int & domainDimension,
                const int & rangeDimension )
{
  Index Is1,Is2,Is3,Ie1,Ie2,Ie3;
  int is[3]={0,0,0}; 
  Range xAxes(0,rangeDimension-1);
  
  for( int dir=0; dir<domainDimension; dir++ )
  {
    if( bc(0,dir)<0 )
    {
      is[dir]=1;
      getBoundaryIndex(indexRange,Start,dir,Is1,Is2,Is3);
      getBoundaryIndex(indexRange,End  ,dir,Ie1,Ie2,Ie3);
      x(Ie1,Ie2,Ie3,xAxes)=x(Is1,Is2,Is3,xAxes);
      x(Ie1+is[0],Ie2+is[1],Ie3+is[2],xAxes)=x(Is1+is[0],Is2+is[1],Is3+is[2],xAxes);
      x(Is1-is[0],Is2-is[1],Is3-is[2],xAxes)=x(Ie1-is[0],Ie2-is[1],Ie3-is[2],xAxes);

      is[dir]=0;
    }
  }
  return 0;
}


int 
equiGridSmoother(const int & domainDimension, 
		 const int & rangeDimension,
		 IntegerArray & indexRange, 
		 IntegerArray & bc, 
		 const int & axis,
		 const realArray & x, 
		 realArray & r,
                 const real arcLengthWeight /* =1. */,
                 const real curvatureWeight /* =1. */,
                 const real areaWeight /* =1. */,
                 int numberOfSmooths /* =0  */)
// ==============================================================================================
/// 
/// \details 
///  Determine new coordinates which equidistribute a weight function
///  based on arclength and curvature
/// 
/// \param numberOfSmooths (input) : number of times to smooth the weight function. Use this if
///    the initial grid is bad.
/// 
/// \param Notes:
///    What about area:   | dArea(i,j)/dArea -1 | 
/// 
// ==============================================================================================
{
  assert( domainDimension>0 && domainDimension<=3 );
  assert( rangeDimension>0 && rangeDimension<=3 );
  assert( axis>=0 && axis<domainDimension );
  
  Index I1,I2,I3,Ib1,Ib2,Ib3;
  
  getIndex(indexRange,I1,I2,I3,+1);  // plus one ghost point
  realArray w(I1,I2,I3),w2(I1,I2,I3),wa(I1,I2,I3);

//  Index J1,J2,J3;
//  getIndex(indexRange,J1,J2,J3);  // interior and boundary

  // equidistribute arclength along this axis
//    int base=I1.getBase(), bound=I1.getBound();
  int is1 = axis==axis1 ? 1 : 0;
  int is2 = axis==axis2 ? 1 : 0;
  int is3 = axis==axis3 ? 1 : 0;
  
  int is[3]={0,0,0}; 

  // *** we assume there are 2 ghost points ****
  if( rangeDimension==1 )
  {
    w(I1,I2,I3)=SQR(x(I1+is1,I2+is2,I3+is3,0)                -x(I1-is1,I2-is2,I3-is3,0)); 
    if( curvatureWeight!=0. )
      w2(I1,I2,I3)=fabs(x(I1+is1,I2+is2,I3+is3,0)-2.*x(I1,I2,I3,0)+x(I1-is1,I2-is2,I3-is3,0))/w(I1,I2,I3);  
    w(I1,I2,I3)=sqrt(w(I1,I2,I3));
  }
  else if( rangeDimension==2 )
  {
    w(I1,I2,I3)=(  SQR(x(I1+is1,I2+is2,I3+is3,0)                -x(I1-is1,I2-is2,I3-is3,0))+
		   SQR(x(I1+is1,I2+is2,I3+is3,1)                -x(I1-is1,I2-is2,I3-is3,1)) );
    if( curvatureWeight!=0. )
    {
      const realArray & x2 = evaluate(x(I1+is1,I2+is2,I3+is3,0)-2.*x(I1,I2,I3,0)+x(I1-is1,I2-is2,I3-is3,0));
      const realArray & y2 = evaluate(x(I1+is1,I2+is2,I3+is3,1)-2.*x(I1,I2,I3,1)+x(I1-is1,I2-is2,I3-is3,1));
      w2(I1,I2,I3)= SQRT(SQR(x2)+SQR(y2))/w(I1,I2,I3);
    }
    w(I1,I2,I3)=sqrt(w(I1,I2,I3));
    // area
    if( areaWeight!=0. )
      wa(I1,I2,I3)  = fabs(  (x(I1+1,I2,I3,0)-x(I1-1,I2,I3,0))*(x(I1,I2+1,I3,1)-x(I1,I2-1,I3,1))-    
			     (x(I1+1,I2,I3,1)-x(I1-1,I2,I3,1))*(x(I1,I2+1,I3,0)-x(I1,I2-1,I3,0)) );
  }
  else 
  {
    w(I1,I2,I3)=(SQR(x(I1+is1,I2+is2,I3+is3,0)                 -x(I1-is1,I2-is2,I3-is3,0))+
		 SQR(x(I1+is1,I2+is2,I3+is3,1)                 -x(I1-is1,I2-is2,I3-is3,1))+
		 SQR(x(I1+is1,I2+is2,I3+is3,2)                 -x(I1-is1,I2-is2,I3-is3,2))) ;
    
    if( curvatureWeight!=0. )
    {
      const realArray & x2 = evaluate(x(I1+is1,I2+is2,I3+is3,0)-2.*x(I1,I2,I3,0)+x(I1-is1,I2-is2,I3-is3,0));
      const realArray & y2 = evaluate(x(I1+is1,I2+is2,I3+is3,1)-2.*x(I1,I2,I3,1)+x(I1-is1,I2-is2,I3-is3,1));
      const realArray & z2 = evaluate(x(I1+is1,I2+is2,I3+is3,2)-2.*x(I1,I2,I3,2)+x(I1-is1,I2-is2,I3-is3,2));
      
      w2(I1,I2,I3)= SQRT(SQR(x2)+SQR(y2)+SQR(z2))/w(I1,I2,I3);
      
      // **** we could pass the ghost points to the DPM ??
      // Set the curvature at physical boundaries to be equal to the first line in
      // since the data point mapping is usually extrapolated.
      for( int dir=0; dir<domainDimension; dir++ )
      {
        for( int side=Start; side<=End; side++ )
	{
	  if( bc(0,dir)>0 )
	  {
	    is[dir]=1-2*side;
	    getBoundaryIndex(indexRange,side,dir,Ib1,Ib2,Ib3);
            w2(Ib1,Ib2,Ib3)=w2(Ib1+is[0],Ib2+is[1],Ib3+is[2]);
            w2(Ib1-is[0],Ib2-is[1],Ib3-is[2])=w2(Ib1,Ib2,Ib3);
	    is[dir]=0;
	  }
	}
      }
    }
    
    w(I1,I2,I3)=SQRT(w(I1,I2,I3));
    
    if( areaWeight!=0. )
    {
      if( domainDimension==2 )
      { // surface area:
        const realArray & v1 =evaluate( (x(I1+1,I2,I3,1)-x(I1-1,I2,I3,1))*(x(I1,I2+1,I3,2)-x(I1,I2-1,I3,2))-
				  (x(I1+1,I2,I3,2)-x(I1-1,I2,I3,2))*(x(I1,I2+1,I3,1)-x(I1,I2-1,I3,1))  );
        const realArray & v2 =evaluate( (x(I1+1,I2,I3,2)-x(I1-1,I2,I3,2))*(x(I1,I2+1,I3,0)-x(I1,I2-1,I3,0))-
				  (x(I1+1,I2,I3,0)-x(I1-1,I2,I3,0))*(x(I1,I2+1,I3,2)-x(I1,I2-1,I3,2))  );
        const realArray & v3 =evaluate( (x(I1+1,I2,I3,0)-x(I1-1,I2,I3,0))*(x(I1,I2+1,I3,1)-x(I1,I2-1,I3,1))-
				  (x(I1+1,I2,I3,1)-x(I1-1,I2,I3,1))*(x(I1,I2+1,I3,0)-x(I1,I2-1,I3,0))  );

	wa(I1,I2,I3)=SQRT( SQR(v1)+SQR(v2)+SQR(v3) );
      }
      else
      {
	wa(I1,I2,I3)=fabs( 
	  (x(I1+1,I2,I3,0)-x(I1-1,I2,I3,0))*(
	    (x(I1,I2+1,I3,1)-x(I1,I2-1,I3,1))*(x(I1,I2,I3+1,2)-x(I1,I2,I3-1,2))-    
	    (x(I1,I2+1,I3,2)-x(I1,I2-1,I3,2))*(x(I1,I2,I3+1,1)-x(I1,I2,I3-1,1)) )
	  -(x(I1+1,I2,I3,1)-x(I1-1,I2,I3,1))*(
	    (x(I1,I2+1,I3,0)-x(I1,I2-1,I3,0))*(x(I1,I2,I3+1,2)-x(I1,I2,I3-1,2))-    
	    (x(I1,I2+1,I3,2)-x(I1,I2-1,I3,2))*(x(I1,I2,I3+1,0)-x(I1,I2,I3-1,0)) )
	  +(x(I1+1,I2,I3,2)-x(I1-1,I2,I3,2))*(
	    (x(I1,I2+1,I3,0)-x(I1,I2-1,I3,0))*(x(I1,I2,I3+1,1)-x(I1,I2,I3-1,1))-    
	    (x(I1,I2+1,I3,1)-x(I1,I2-1,I3,1))*(x(I1,I2,I3+1,0)-x(I1,I2,I3-1,0)) )
	  );
      }
    }
  }

  real eps = REAL_EPSILON;
#define AREA(i1,i2,i3) (wa(i1,i2,i3)*areaWeight/(eps+max(wa(i1,i2,i3))))
#define CURVATURE(i1,i2,i3) (w2(i1,i2,i3)*(curvatureWeight/(eps+max(w2(i1,i2,i3)))))
  // normalize so max(arcLength)=1 and max(curvature)=1
  if( axis==axis1 )
  {
    // we need to loop so the normalization is by row
    for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
      for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
        if( arcLengthWeight!=0. )
          w(I1,i2,i3)=1.+w(I1,i2,i3)*(arcLengthWeight/(eps+max(w(I1,i2,i3))));
        else
          w(I1,i2,i3)=1.;
        if( curvatureWeight!=0. )
          w(I1,i2,i3)+=CURVATURE(I1,i2,i3);
	if( areaWeight!=0. )
	  w(I1,i2,i3)+=AREA(I1,i2,i3);
//	  w(I1,i2,i3)+=fabs(wa(I1,i2,i3)-1.)*areaWeight;
      }
    }
  }
  else if( axis==axis2 )
  {
    for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
      for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
      {
        if( arcLengthWeight!=0. )
          w(i1,I2,i3)=1.+w(i1,I2,i3)*(arcLengthWeight/(eps+max(w(i1,I2,i3))));
        else
          w(i1,I2,i3)=1.;
        if( curvatureWeight!=0. )
          w(i1,I2,i3)+=CURVATURE(i1,I2,i3);
	if( areaWeight!=0. )
	  w(i1,I2,i3)+=AREA(i1,I2,i3);
      }
    }
  }
  else
  {
    for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
    {
      for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
      {
        if( arcLengthWeight!=0. )
          w(i1,i2,I3)=1.+w(i1,i2,I3)*(arcLengthWeight/(eps+max(w(i1,i2,I3))));
        else
          w(i1,i2,I3)=1.;
        if( curvatureWeight!=0. )
          w(i1,i2,I3)+=CURVATURE(i1,I2,I3);
	if( areaWeight!=0. )
	  w(i1,i2,I3)+=AREA(i1,i2,I3);
      }
    }
  }
    
  // smooth the weight function
  getIndex(indexRange,I1,I2,I3);  // interior and boundary
  
  periodicUpdate(w,indexRange,bc,domainDimension,1);
  for( int it=0; it<numberOfSmooths; it++ )
  {
    if( domainDimension==1 )
    {
      w(I1,I2,I3)=.5*(w(I1+1,I2,I3)+w(I1-1,I2,I3));
    }
    else if( domainDimension==2 )
    {
      w(I1,I2,I3)=.25*(w(I1+1,I2,I3)+w(I1-1,I2,I3)+
		       w(I1,I2+1,I3)+w(I1,I2-1,I3));
    }
    else
    {
      w(I1,I2,I3)=(w(I1+1,I2,I3)+w(I1-1,I2,I3)+
		   w(I1,I2+1,I3)+w(I1,I2-1,I3)+
		   w(I1,I2,I3+1)+w(I1,I2,I3-1))*(1./6.);
    }
    periodicUpdate(w,indexRange,bc,domainDimension,1);
  }
  
  // now equidistribute:
  if( axis==axis1 )
  {
    realArray ww(I1),rr(I1);
    for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
      for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
	ww=w(I1,i2,i3);
	equidistribute( ww,rr );
	r(I1,i2,i3,axis1)=rr;
	if( domainDimension>1 )
	  r(I1,i2,i3,axis2)=(real(i2)-I2.getBase())/(I2.getBound()-I2.getBase());   // ** use seqAdd ***
	if( domainDimension>2 )
	  r(I1,i2,i3,axis3)=(real(i3)-I3.getBase())/(I3.getBound()-I3.getBase());
	
      }
    }
  }
  else if( axis==axis2 )
  {
    realArray ww(I2),rr(I2);
    for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
      for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
      {
	ww.reshape(1,I2);
	ww(0,I2)=w(i1,I2,i3);
	ww.reshape(I2);
        rr.reshape(I2);
	equidistribute( ww,rr );
	rr.reshape(1,I2);
	r(i1,I2,i3,axis2)=rr(0,I2);

	r(i1,I2,i3,axis1)=(real(i1)-I1.getBase())/(I1.getBound()-I1.getBase());
	if( domainDimension>2 )
	  r(i1,I2,i3,axis3)=(real(i3)-I3.getBase())/(I3.getBound()-I3.getBase());
      }
    }
  }
  else
  {
    realArray ww(I3),rr(I3);
    for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
    {
      for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
      {
	ww.reshape(1,1,I3);
	ww(0,0,I3)=w(i1,i2,I3);
	ww.reshape(I3);
        rr.reshape(I3);
	equidistribute( ww,rr );
	rr.reshape(1,1,I3);
	r(i1,i2,I3,axis3)=rr(0,0,I3);

	r(i1,i2,I3,axis1)=(real(i1)-I1.getBase())/(I1.getBound()-I1.getBase());
	r(i1,i2,I3,axis2)=(real(i2)-I2.getBase())/(I2.getBound()-I2.getBase());
      }
    }
  }
/* ---
  char buff[80];
  display(w(I1,I2,I3),sPrintF(buff,"here is the weight function w, axis=%i",axis));
  if( curvatureWeight!=0. )
    display(w(I1,I2,I3),sPrintF(buff,"here is the curvature function w2, axis=%i",axis));

  display(r(I1,I2,I3,Index(0,domainDimension)),sPrintF(buff,"here is r, axis=%i",axis));
  display(x(I1,I2,I3,Index(0,rangeDimension)),sPrintF(buff,"here is x, axis=%i",axis));
--- */    
    
  return 0;
}



    

#include "DataPointMapping.h"
#include "GenericGraphicsInterface.h"
#include "CompositeSurface.h"

int 
equiGridSmoother(Mapping & map,
                 DataPointMapping & dpm, 
                 GenericGraphicsInterface & gi, 
                 GraphicsParameters & parameters,
                 IntegerArray & bc,
                 real & arcLengthWeight,
                 real & curvatureWeight,
                 real & areaWeight )
// ========================================================================================================
/// \details 
///     This is an interactive routine that can used to smooth a DataPointMapping that sits
///   on an underlying Mapping (such as a HyperbolicSurfaceMapping that is created on some other surface).
/// 
///   The grid is smoothed by attempting to equidistribute a weight function that is a combination of
///  measures of the arclength, curvature and area.
///     
/// \param map (input) : This is the Mapping that really defines the surface or the boundary of a 3D volume.
///        This Mapping is used to project back onto after each smoothing step.
///  dpm (input) : This is the DataPointMapping that ...
// ========================================================================================================
{

  enum BoundaryConditions
  {
    pointsFixed=1,
    pointsSlide,
    boundaryIsSmoothed
  };
      
  const int domainDimension = dpm.getDomainDimension();
  const int rangeDimension = dpm.getRangeDimension();

  IntegerArray indexRange(2,3);
  // ** IntegerArray bc(2,3);   bc=pointsFixed;
  indexRange=0;
  int axis;
  for( axis=0; axis<domainDimension; axis++ )
  {
    indexRange(End,axis)=dpm.getGridDimensions(axis)-1;
    if( dpm.getIsPeriodic(axis) )
    {
      bc(Range(0,1),axis)=-1;    // *****
      printf(" dpm.getIsPeriodic(%i) = %i \n",axis,dpm.getIsPeriodic(axis));
    }
  }
  
  arcLengthWeight=1.;
  curvatureWeight=.5;
  areaWeight=.5;
  
  char buff[80];
  aString answer,line;
  aString menu[] = { "smooth",
                    "number of iterations",
                    "boundary conditions for smoothing",
                    "arclength weight",
                    "curvature weight",
                    "area weight",
                    "number of weight smooths",
                    "exit",
                    "" };                       // empty string denotes the end of the menu

  int numberOfIterations=1;
  int totalIterations=0;
  int numberOfWeightSmooths=2;
  int numberOfLaplacianSmooths=0;
  
  // I1,I2,I3 : interior points plus periodic edges
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  ::getIndex(indexRange,I1,I2,I3,-1); 
  // J1,J2,J3 : interior + bndry pts + extra lines on periodic edges
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  ::getIndex(indexRange,J1,J2,J3);  
  // K1,K2,K3 : interior + bndry pts 
  Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];
  ::getIndex(indexRange,K1,K2,K3);  
  for( axis=0; axis<domainDimension; axis++ )
  {
    if( bc(Start,axis)<0 ) // or if points can slide ***
    {
      Iv[axis]=Range(indexRange(Start,axis)  ,indexRange(End,axis));
      Jv[axis]=Range(indexRange(Start,axis)-1,indexRange(End,axis)+1);
    }
  }


  realArray r(J1,J2,J3,domainDimension),x(J1,J2,J3,rangeDimension);
  Range xAxes(0,rangeDimension-1);
  
  intArray subSurfaceIndex;
  realArray rProject, xProject, xrProject;
  const int n = J1.length()*J2.length()*J3.length();
  if( map.getClassName()=="CompositeSurface" )
  {
    subSurfaceIndex.redim(n);
    subSurfaceIndex=-1;
    rProject.redim(n,domainDimension); rProject=-1.;
    xProject.redim(n,rangeDimension); 
    xrProject.redim(n,rangeDimension,domainDimension);
  }
  realArray rr,xx;
  if( domainDimension==rangeDimension )
  {
    rr.redim(J1,1,1,domainDimension-1);  // only valid for one boundary
    xx.redim(J1,1,1,rangeDimension);
  }
  else
  {
    rr.redim(n,domainDimension);
    xx.redim(J1,J2,J3,rangeDimension);
  }

  
  Index Is1,Is2,Is3,Ie1,Ie2,Ie3;
  
  for(;;)
  {
    gi.erase();
    parameters.set(GI_TOP_LABEL,map.getName(Mapping::mappingName)+sPrintF(buff," (iteration=%i)",totalIterations));
    PlotIt::plot(gi,dpm,parameters); 
    gi.redraw(TRUE);

    gi.getMenuItem(menu,answer);               

    if( answer=="smooth" )
    {
      // Here is the array of grid points
      const realArray & xy = dpm.getGrid();  // returns a reference. *******

      for( int it=0; it<numberOfIterations; it++ )
      {
        realArray xyOld;
	xyOld=xy;
	for( axis=axis1; axis<domainDimension; axis++ )
	{
          // redistribute points in the direction axis:
	  equiGridSmoother(domainDimension,rangeDimension,indexRange,bc, axis,xy,r,
			   arcLengthWeight,curvatureWeight,areaWeight,numberOfWeightSmooths);

          // periodic boundary conditions
          // note that only r(.,.,.,axis) is changed from uniform
          for( int dir=0; dir<domainDimension; dir++ )
	  {
            if( dir!=axis && bc(0,dir)<0 )
	    {
	      getBoundaryIndex(indexRange,Start,dir,Is1,Is2,Is3);
	      getBoundaryIndex(indexRange,End  ,dir,Ie1,Ie2,Ie3);
//	      r(Ie1,Ie2,Ie3,axis)=r(Is1,Is2,Is3,axis);
	    }
	  }
	  
	  r.reshape(n,domainDimension);
	  x.reshape(n,rangeDimension);
	  dpm.map(r,x);
	  r.reshape(J1,J2,J3,domainDimension);
	  x.reshape(J1,J2,J3,rangeDimension);

	  for( int jt=0; jt<numberOfLaplacianSmooths; jt++ )
	  {
            periodicUpdate(x,indexRange,bc,domainDimension,rangeDimension);

	    real omega=.25;
	    if( domainDimension==2 )
	    {
              if( TRUE )
	      {
      	        x(I1,I2,I3,xAxes)+=(omega*.25)*(x(I1+1,I2,I3,xAxes)+x(I1-1,I2,I3,xAxes)
					       +x(I1,I2+1,I3,xAxes)+x(I1,I2-1,I3,xAxes)-4.*x(I1,I2,I3,xAxes));
	      }
              /* ----
	      else
	      {
		// fourth order smoothing
		x(I1,I2,I3,xAxes)+=(omega/12.)*(
		  -(x(I1+2,I2,I3,xAxes)+x(I1-2,I2,I3,xAxes)+x(I1,I2+2,I3,xAxes)+x(I1,I2-2,I3,xAxes))
		  +4.*(x(I1+1,I2,I3,xAxes)+x(I1-1,I2,I3,xAxes)+x(I1,I2+1,I3,xAxes)+x(I1,I2-1,I3,xAxes))
		  -12.*x(I1,I2,I3,xAxes));
	      }
              ---- */
	    }
	    else
	    {
	      x(I1,I2,I3,xAxes)=(x(I1+1,I2,I3,xAxes)+x(I1-1,I2,I3,xAxes)
				 +x(I1,I2+1,I3,xAxes)+x(I1,I2-1,I3,xAxes)
				 +x(I1,I2,I3+1,xAxes)+x(I1,I2,I3-1,xAxes))*(1./6.);
	    }
            periodicUpdate(x,indexRange,bc,domainDimension,rangeDimension);

	  }

	  if( domainDimension==rangeDimension )
	  {
	    // project boundaries back onto the original boundaries.
	    int j2=J2.getBase();
	    int j3=J3.getBase();
	    xx(J1,j2,j3,xAxes)=x(J1,j2+1,j3,xAxes);  // 2d only
	    xx.reshape(J1,xAxes);
	    rr.reshape(J1,1);
	    if( map.getClassName()=="CompositeSurface" )
	    {
	      CompositeSurface & cs = (CompositeSurface&) map;
	      cs.project(subSurfaceIndex,xx,rProject,xProject,xrProject);
	      xx=xProject;
	    }
	    else
	    {
	      map.inverseMap(xx,rr);   // project onto original surface
	      map.map(rr,xx);
	    }
	    xx.reshape(J1,1,1,xAxes);
	    x(J1,j2,j3,xAxes)=.5*(x(J1,j2,j3,xAxes)+xx(J1,j2,j3,xAxes));  // **** fix this ****
	  }
	  else
	  {
	    // project the grid onto the original surface
	    xx(J1,J2,J3,xAxes)=x(J1,J2,J3,xAxes); 
	    xx.reshape(n,xAxes);
	    if( map.getClassName()=="CompositeSurface" )
	    {
	      CompositeSurface & cs = (CompositeSurface&) map;
	      cs.project(subSurfaceIndex,xx,rProject,xProject,xrProject);
	      xx=xProject;
	    }
	    else
	    {
	      map.inverseMap(xx,rr);   // project onto original surface
	      map.map(rr,xx);
	    }
	    xx.reshape(J1,J2,J3,xAxes);
            // periodic BC's ??
	    
	    x(J1,J2,J3,xAxes)=xx(J1,J2,J3,xAxes); 

            periodicUpdate(x,indexRange,bc,domainDimension,rangeDimension);

	  }
	  dpm.setDataPoints(x(K1,K2,K3,xAxes),3,domainDimension);

	}
	real xDiff=max(fabs(xy(K1,K2,K3,xAxes)-xyOld(K1,K2,K3,xAxes)));
	printf("Iteration %i, maximum change in x = %e \n",totalIterations,xDiff);
      
	totalIterations++;
      } // end for it
    }
    else if( answer=="number of iterations" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the number of iterations (current=%i)",numberOfIterations));
      if( line!="" ) sScanF( line,"%i",&numberOfIterations);
    }
    else if( answer=="arclength weight" )
    {
      gi.inputString(line,sPrintF(buff,"Enter weight for arclength (current=%e)",arcLengthWeight));
      if( line!="" ) sScanF( line,"%e",&arcLengthWeight);
    }
    else if( answer=="curvature weight" )
    {
      gi.inputString(line,sPrintF(buff,"Enter weight for curvature (current=%e)",curvatureWeight));
      if( line!="" ) sScanF( line,"%e",&curvatureWeight);
    }
    else if( answer=="area weight" )
    {
      gi.inputString(line,sPrintF(buff,"Enter weight for area (current=%e)",areaWeight));
      if( line!="" ) sScanF( line,"%e",&areaWeight);
    }
    else if( answer=="number of weight smooths" )
    {
      gi.inputString(line,sPrintF(buff,"Enter number of weight function smooths (current=%i)",numberOfWeightSmooths));
      if( line!="" ) sScanF( line,"%i",&numberOfWeightSmooths);
    }
    else if( answer=="boundary conditions for smoothing" )
    {
      gi.outputString("Boundary conditions: 1=points fixed on boundary");
      gi.outputString("                   : 2=points slide on boundary to be normal");
      gi.outputString("                   : 3=boundary is smoothed");
    }
    else if( answer=="exit" )
    {
      break;
    }
    else
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
    }
  }
  return 0;
}
