//#define BOUNDS_CHECK
#include "NurbsMapping.h"
#include "display.h"
#include "ArraySimple.h"
#include "ParallelUtility.h"
#include "Inverse.h"

real nurbsEvalTime=0.;



// maximum order for nurbs (p1+1) <= MAX_ORDER
#define MAX_ORDER 14

#define NURBSEVAL EXTERN_C_NAME(nurbseval)

extern "C"
void NURBSEVAL ( int *nder,
		 int *ddim,
		 int *rdim,
		 int *n,
		 int *p,
		 double *knts,
		 int *kbnds,
		 double *cptsw,
		 int *cpbnds,
		 double *u,
		 int *nu,
		 double *ders );

bool NurbsMapping::useScalarEvaluation=true;   // use optimised scalar version by default now

const realArray& NurbsMapping::
getGrid(MappingParameters & params /* =Overture::nullMappingParameters() */,
	bool includeGhost /* =false */ )
{
  // ** todo: write an optimized version ***
  return Mapping::getGrid(params,includeGhost);
}




int NurbsMapping::
findSpan(const int & n ,
         const int & p,
         const real & u,
         const RealArray & knot_ )
//===========================================================================
/// \brief  Protected routine.
/// 
///  Determine the knot span index
/// 
///  Assume the knot is clamped (nonperiodic or open):
///  \begin{verbatim}
///    knot = { u_0,u_0,...,u_0, u_1, ..., u_n, u_{n+1}, u_{n+1} }
///               0   1  ... p    p+1 ...   m-p-1 ...      m
///  \end{verbatim}
/// 
/// \param n,p (input) : n=m-p-1, and p=degree of the nurbs
/// \param u (input) : find index with knot(index) $\le$ u < knot(index+1)
//===========================================================================
{
  const real *knotc = knot_.getDataPointer();
#define knot(i) knotc[(i)]
  if( u>=knot(n+1) )
      return n;
  if( u<=knot(p) )
      return p;
  // binary search:
  int low =p, high=n+1, mid=(low+high)/2;
  while( u<knot(mid) || u>=knot(mid+1) )
  {
    if( u<knot(mid) )
      high=mid;
    else
      low=mid;
    mid=(low+high)/2;
  }
  return mid;
#undef knot
}

void NurbsMapping::
basisFuns(const int & i,
	  const real & u,
          const int & p,
          const RealArray & knot,
          RealArray & basis )
//===========================================================================
/// \brief  Protected routine. Compute the non-vanishing basis functions
/// \param i,u,p,knot (input):
/// \param basis(0:p) : 
/// 
//===========================================================================
{
  real saved,temp;

 real leftp[maximumOrder], rightp[maximumOrder];
#define LEFT(i) leftp[i]
#define RIGHT(i) rightp[i]
 
  basis(0)=1.;
  for( int j=1; j<=p; j++ )
  {
    LEFT(j)=u-knot(i+1-j);
    RIGHT(j)=knot(i+j)-u;
    saved=0.;
    for( int r=0; r<j; r++ )
    {
      temp=basis(r)/(RIGHT(r+1)+LEFT(j-r));
      basis(r)=saved+RIGHT(r+1)*temp;
      saved=LEFT(j-r)*temp;
    }
    basis(j)=saved;
  }
}

void NurbsMapping::
dersBasisFuns(const int & i,
	      const real & u,
	      const int & p,
	      const int & order,
	      const RealArray & knot_,
	      real *dersp )
//===========================================================================
/// \brief  Protected routine.
/// 
///   Compute the non-vanishing basis functions and their derivatives up to order d
/// \param i,u,p,knot (input):
/// \param order (input) order of derivative ( order<=p )
/// \param ders(0:d,0:p) : 
/// 
//===========================================================================
{
  real saved,temp;
  int j,k,j1,j2,r;
  
  real ndup[maximumOrder][maximumOrder];  // [p+1][p+1]
  real ap[2][maximumOrder];
  real leftp[maximumOrder], rightp[maximumOrder];
  
  const real *knotc = knot_.getDataPointer();
#define knot(i) knotc[(i)]

#define NDU(i,j) ndup[i][j]
#define A(i,j) ap[i][j]
#define DERS(i,j) dersp[(i)+2*(j)]

  NDU(0,0)=1.;
  for( j=1; j<=p; j++ )
  {
    LEFT(j)=u-knot(i+1-j);
    RIGHT(j)=knot(i+j)-u;
    saved=0.;
    for( r=0; r<j; r++ )
    {
      NDU(j,r)=RIGHT(r+1)+LEFT(j-r);
      temp=NDU(r,j-1)/NDU(j,r);
      
      NDU(r,j)=saved+RIGHT(r+1)*temp;
      saved=LEFT(j-r)*temp;
    }
    NDU(j,j)=saved;
  }
  for( j=0; j<=p; j++ )
    DERS(0,j)=NDU(j,p);  // basis function
    
  if( order==0 )
    return;
  
  // compute derivatives
  for( r=0; r<=p; r++ )
  {
    int s1=0, s2=1;  // alternate rows in a
    A(0,0)=1.;
    // compute k'th derivative
    for( k=1; k<=order; k++ )
    {
      real d=0.;
      int rk=r-k, pk=p-k;
      if( r>=k )
      {
	A(s2,0)=A(s1,0)/NDU(pk+1,rk);
	d=A(s2,0)*NDU(rk,pk);
      }
      j1 = rk >= -1 ? 1 : -rk;
      j2 = (r-1<=pk) ? k-1 : p-r;
      for( j=j1; j<=j2; j++ )
      {
	A(s2,j)=(A(s1,j)-A(s1,j-1))/NDU(pk+1,rk+j);
	d+=A(s2,j)*NDU(rk+j,pk);
      }
      if( r<=pk )
      {
	A(s2,k)= -A(s1,k-1)/NDU(pk+1,r);
	d+=A(s2,k)*NDU(r,pk);
      }
      DERS(k,r)=d;
      j=s1; s1=s2; s2=j;  // switch rows
    }
  }
  r=p;
  for( k=1; k<=order; k++ )
  {
    for( j=0; j<=p; j++ ) 
      DERS(k,j)*=r;
    r*=p-k;
  }
    
#undef NDU
#undef A
#undef LEFT
#undef RIGHT
#undef DERS
#undef knot
}

real nurbTimeEvaluate=0.;
real nurbsBasisTime=0.;

void NurbsMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
//=====================================================================================
/// \brief  Evaluate the nurbs and/or derivatives. 
//=====================================================================================
{
  #ifndef USE_PPP
    mapS(r,x,xr,params);
  #else
    realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(x,xLocal);
    realSerialArray rLocal;  getLocalArrayWithGhostBoundaries(r,rLocal);
    realSerialArray xrLocal; getLocalArrayWithGhostBoundaries(xr,xrLocal);
    mapS(rLocal,xLocal,xrLocal,params);
  #endif
}



void NurbsMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
//=====================================================================================
/// \brief  Evaluate the nurbs and/or derivatives. 
//=====================================================================================
{
  if( !initialized )
  {
    initialize();
    // mappingNeedsToBeReinitialized=TRUE; // re-init mapping and inverse
    reinitialize(); 
  }
  if( params.coordinateType != cartesian )
    cerr << "NurbsMapping::map - coordinateType != cartesian " << endl;

  assert( !inverseIsDistributed );

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  // ******************* wdh 080929 *********** turn this on for testing: 
  // if( domainDimension==3 )
  //  use_kk_nrb_eval=true;
  
  if( bound-base > 1 && !useScalarEvaluation && !use_kk_nrb_eval && domainDimension!=3 ) 
  { // use vectorized version
    mapVector(r,x,xr,params);
    return;
  }


  int nw=rangeDimension;  // position of the weight in the cPoint array
  real r0,r1;
  Range R(0,rangeDimension-1);
  // control points includeds the weights as a last component, if the weights are nonuniform
  // then we must compute the b-spline for the weight component too:
  Range Rw= nonUniformWeights ? Range(0,rangeDimension) : R;
  
  real rScale[3];
  bool reScale[3];
  RealArray rr(I,domainDimension);

  const real * rp = r.Array_Descriptor.Array_View_Pointer1;
  const int rDim0=r.getRawDataSize(0);
#undef R
#define R(i0,i1) rp[i0+rDim0*(i1)]
  real * rrp = rr.Array_Descriptor.Array_View_Pointer1;
  const int rrDim0=rr.getRawDataSize(0);
#undef RR
#define RR(i0,i1) rrp[i0+rrDim0*(i1)]
  real * xp = x.Array_Descriptor.Array_View_Pointer1;
  const int xDim0=x.getRawDataSize(0);
#undef X
#define X(i0,i1) xp[i0+xDim0*(i1)]
  real * xrp = xr.Array_Descriptor.Array_View_Pointer2;
  const int xrDim0=xr.getRawDataSize(0);
  const int xrDim1=xr.getRawDataSize(1);
#undef XR
#define XR(i0,i1,i2) xrp[i0+xrDim0*(i1+xrDim1*(i2))]



  int axis;
  for( axis=0; axis<domainDimension; axis++ )
  {
    rScale[axis]=rEnd[axis]-rStart[axis];
    reScale[axis] = rScale[axis]!=1. || rStart[axis]!=0.;
    if( nurbsIsPeriodic[axis]==functionPeriodic || reScale[axis] )  // ****** we need to add shift for derivative periodic
    {
      // if( reScale[axis] )
      //   printf("NurbsMapping: axis=%i, rScale=%e nurbsIsPeriodic=%i\n",axis,rScale[axis],nurbsIsPeriodic[axis]);
    
      if( !nurbsIsPeriodic[axis] )
      {
	for( int i=base; i<=bound; i++ )
	  RR(i,axis)=rScale[axis]*R(i,axis)+rStart[axis];  // rescale non-periodic
      }
      
      else
      {
	if( reScale[axis] )
	{
	  for( int i=base; i<=bound; i++ )
	    RR(i,axis)=fmod(rScale[axis]*R(i,axis)+rStart[axis]+2.,1.);  // we enforce rStart>=-1.
	}
	else
	{
	  for( int i=base; i<=bound; i++ )
	    RR(i,axis)=fmod(R(i,axis)+1.,1.);   // map to [0,1] ** assumes r>=-1.
	}
      }
    }
    else
    {
      for( int i=base; i<=bound; i++ )
	RR(i,axis)=R(i,axis);
    }
    
  }

  // optionally use the fortran nurbs evaluator from Eleven
  if( use_kk_nrb_eval )
    {
      real time0 = getCPU();

      ArraySimpleFixed<int,4,1,1,1> cpBounds,nn,p;
      ArraySimpleFixed<int,2,4,1,1> kntBounds;
      kntBounds = 0;
      cpBounds = 0;
      p = nn = 0;
      for ( int i=0; i<domainDimension; i++ )
	{
	  cpBounds[i] = cPoint.getBound(i);
	  p[i] = getOrder(i);
	  nn[i] = getNumberOfKnots(i)-p[i] - 2;
	}
      
      kntBounds(1,0) = uKnot.getBound(0);
      if ( domainDimension>1 )
	{
	  kntBounds(0,1) = vKnot.getDataPointer() - uKnot.getDataPointer();
	  kntBounds(1,1) = kntBounds(0,1) + vKnot.getBound(0);

	  if ( domainDimension>2 )
	    {
	      kntBounds(0,2) = wKnot.getDataPointer() - uKnot.getDataPointer();
	      kntBounds(1,2) = kntBounds(0,2) + wKnot.getBound(0);
	    }
	}
      int nw=rangeDimension;  // position of the weight
      Range R1(0,n1);

      // cout<<"base, bound "<<base<<", "<<bound<<endl;
//       cout<<"KNTBOUNDS = "<<kntBounds<<endl;
//       cout<<"cpBounds  = "<<cpBounds<<endl;
//      r.display();
      for ( int i=domainDimension; i<4; i++ )
	kntBounds(0,i) = kntBounds(1,i) = kntBounds(1,domainDimension-1);

      int nder = computeMapDerivative ? 1 : 0;
      int nToEval = bound-base+1;
      ArraySimple<real> ders(nToEval,int(pow(nder+1,domainDimension)),rangeDimension);
      ders=0.; 
      real time=getCPU();
      NURBSEVAL( &nder, &domainDimension, &rangeDimension, nn.ptr(), p.ptr(), 
		 uKnot.getDataPointer(), kntBounds.ptr(),
		 cPoint.getDataPointer(), cpBounds.ptr(), (real*)rr.getDataPointer(), 
		 &nToEval, ders.ptr() );

      nurbsEvalTime+= getCPU()-time0;
      
      // printf("Time for NURBSEVAL=%8.2e (s)\n",getCPU()-time0);

      //      cout<<"RESULT "<<ders<<endl;
      if ( computeMap )
	{
	  for ( int i=0; i<nToEval; i++ )
	    {
	      for ( int a=0; a<rangeDimension; a++ )
		X(i,a) = ders(i,0,a);
	    }
	}
      if ( computeMapDerivative )
	{
	  switch ( domainDimension ) {
	  case 1:
	    for ( int i=0; i<nToEval; i++ )
	      {
		for ( int a=0; a<rangeDimension; a++ )
		  { // XXX hard coded for first derivative only
		    XR(i,a,0) = rScale[0]*ders(i,1,a);
		  }
	      }
	    break;
	  case 2:
	    ders.resize(nToEval,nder+1,nder+1,rangeDimension);
	    for ( int i=0; i<nToEval; i++ )
	      {
		for ( int a=0; a<rangeDimension; a++ )
		  { // XXX hard coded for first derivative only
		    XR(i,a,0) = rScale[0]*ders(i,1,0,a);
		    XR(i,a,1) = rScale[1]*ders(i,0,1,a);
		  }
	      }
	    break;
	  case 3:
	    ders.resize(nToEval,nder+1,nder+1,nder+1,rangeDimension);
	    for ( int i=0; i<nToEval; i++ )
	      {
		for ( int a=0; a<rangeDimension; a++ )
		  { // XXX hard coded for first derivative only
		    XR(i,a,0) = rScale[0]*ders(i,1,0,0,a);
		    XR(i,a,1) = rScale[1]*ders(i,0,1,0,a);
		    XR(i,a,2) = rScale[2]*ders(i,0,0,1,a);
		  }
	      }
	    break;
	  }
	}
      
      
      nurbTimeEvaluate+=getCPU()-time0;
      return;
    }


  real time0=getCPU();
  real udersp[2*maximumOrder];
  real xv0,xv1,xv2,xv3;
  real xrv0,xrv1,xrv2,xrv3;

  real *cPointp = cPoint.getDataPointer();
  const int ndcp = cPoint.getLength(0);
#define UDERS(i,j) udersp[(i)+2*(j)]
#define CP(i,j) cPointp[(i)+ndcp*(j)]

  if( domainDimension==1 )
  {
    int order= computeMapDerivative ? 1 : 0;  // compute derivatives up to this order
    int span; 
    rScale[0]=rEnd[axis1]-rStart[axis1];
    reScale[0] = rScale[axis1]!=1. || rStart[axis1]!=0.;
    for( int i=base; i<=bound; i++ )
    {
      r0=RR(i,0);

//      real time1=getCPU();
      span = findSpan( n1,p1,r0,uKnot );
      dersBasisFuns(span,r0,p1,order,uKnot,udersp); 
//      nurbsBasisTime+=getCPU()-time1;

      if( computeMap || nonUniformWeights )
      {
        if( rangeDimension==2 )
	{
          if( !nonUniformWeights )
	  {
	    xv0=0.; xv1=0.;
	    for( int ii=0; ii<=p1; ii++ )
	    {
	      xv0+=UDERS(0,ii)*CP(span-p1+ii,0);
	      xv1+=UDERS(0,ii)*CP(span-p1+ii,1);
	    }
	  }
	  else
	  {
	    switch (p1 )
	    {
	    case 3:
	      xv0=(UDERS(0,0)*CP(span-p1+0,0)+
		   UDERS(0,1)*CP(span-p1+1,0)+
		   UDERS(0,2)*CP(span-p1+2,0)+
		   UDERS(0,3)*CP(span-p1+3,0));
	      xv1=(UDERS(0,0)*CP(span-p1+0,1)+
		   UDERS(0,1)*CP(span-p1+1,1)+
		   UDERS(0,2)*CP(span-p1+2,1)+
		   UDERS(0,3)*CP(span-p1+3,1));
	      xv2=(UDERS(0,0)*CP(span-p1+0,2)+
		   UDERS(0,1)*CP(span-p1+1,2)+
		   UDERS(0,2)*CP(span-p1+2,2)+
		   UDERS(0,3)*CP(span-p1+3,2));
	      break;
	    default:
	    {
   	      xv0=0.; xv1=0.; xv2=0.;
	      for( int ii=0; ii<=p1; ii++ )
	      {
		xv0+=UDERS(0,ii)*CP(span-p1+ii,0);
		xv1+=UDERS(0,ii)*CP(span-p1+ii,1);
		xv2+=UDERS(0,ii)*CP(span-p1+ii,2);
	      }
	    }
	    }
	    
            xv0/=xv2;
            xv1/=xv2;
	  }
          if( computeMap )
	  {
	    X(i,0)=xv0;
	    X(i,1)=xv1;
	  }
	}
	else if( rangeDimension==3 )
	{
          if( !nonUniformWeights )
	  {
	    xv0=0.; xv1=0.; xv2=0.;
	    for( int ii=0; ii<=p1; ii++ )
	    {
	      xv0+=UDERS(0,ii)*CP(span-p1+ii,0);
	      xv1+=UDERS(0,ii)*CP(span-p1+ii,1);
	      xv2+=UDERS(0,ii)*CP(span-p1+ii,2);
	    }
	  }
	  else
	  {
	    switch (p1 )
	    {
	    case 3:
	      xv0=(UDERS(0,0)*CP(span-p1+0,0)+
		   UDERS(0,1)*CP(span-p1+1,0)+
		   UDERS(0,2)*CP(span-p1+2,0)+
		   UDERS(0,3)*CP(span-p1+3,0));
	      xv1=(UDERS(0,0)*CP(span-p1+0,1)+
		   UDERS(0,1)*CP(span-p1+1,1)+
		   UDERS(0,2)*CP(span-p1+2,1)+
		   UDERS(0,3)*CP(span-p1+3,1));
	      xv2=(UDERS(0,0)*CP(span-p1+0,2)+
		   UDERS(0,1)*CP(span-p1+1,2)+
		   UDERS(0,2)*CP(span-p1+2,2)+
		   UDERS(0,3)*CP(span-p1+3,2));
	      xv3=(UDERS(0,0)*CP(span-p1+0,3)+
		   UDERS(0,1)*CP(span-p1+1,3)+
		   UDERS(0,2)*CP(span-p1+2,3)+
		   UDERS(0,3)*CP(span-p1+3,3));
	      break;
	    default:
	    {
  	      xv0=0.; xv1=0.; xv2=0.; xv3=0.;
	      for( int ii=0; ii<=p1; ii++ )
	      {
		xv0+=UDERS(0,ii)*CP(span-p1+ii,0);
		xv1+=UDERS(0,ii)*CP(span-p1+ii,1);
		xv2+=UDERS(0,ii)*CP(span-p1+ii,2);
		xv3+=UDERS(0,ii)*CP(span-p1+ii,3);
	      }
	    }
	    }
	    
            xv0/=xv3;
            xv1/=xv3;
            xv2/=xv3;
	  }
          if( computeMap )
	  {
	    X(i,0)=xv0;
	    X(i,1)=xv1;
	    X(i,2)=xv2;
	  }
	}
        else if( rangeDimension==1 ) // *wdh* 040530
	{
          if( !nonUniformWeights )
	  {
	    xv0=0.; 
	    for( int ii=0; ii<=p1; ii++ )
	    {
	      xv0+=UDERS(0,ii)*CP(span-p1+ii,0);
	    }
	  }
	  else
	  {
	    switch (p1 )
	    {
	    case 3:
	      xv0=(UDERS(0,0)*CP(span-p1+0,0)+
		   UDERS(0,1)*CP(span-p1+1,0)+
		   UDERS(0,2)*CP(span-p1+2,0)+
		   UDERS(0,3)*CP(span-p1+3,0));
	      xv1=(UDERS(0,0)*CP(span-p1+0,1)+
		   UDERS(0,1)*CP(span-p1+1,1)+
		   UDERS(0,2)*CP(span-p1+2,1)+
		   UDERS(0,3)*CP(span-p1+3,1));
	      break;
	    default:
	    {
   	      xv0=0.; xv1=0.; 
	      for( int ii=0; ii<=p1; ii++ )
	      {
		xv0+=UDERS(0,ii)*CP(span-p1+ii,0);
		xv1+=UDERS(0,ii)*CP(span-p1+ii,1);
	      }
	    }
	    }
	    
            xv0/=xv1;
	  }
          if( computeMap )
	  {
	    X(i,0)=xv0;
	  }
	}
	else
	{
	  printf("NurbsMapping::map:ERROR: rangeDimension=%i\n",rangeDimension);
	  throw "error";
	}
// 	  xv(0,Rw)=0.;
// 	  for( int ii=0; ii<=p1; ii++ )
// 	    xv(0,Rw)+=UDERS(0,ii)*cPoint(span-p1+ii,Rw);
// 	  if( nonUniformWeights )
// 	    xv(0,R)*=(1./xv(0,nw));

// 	  if( computeMap )
// 	    x(i,R)=xv(0,R);
	
      }

      if( computeMapDerivative )
      {
        if( rangeDimension==2 )
	{
          if( nonUniformWeights )
	  {
	    xrv0=0.; xrv1=0.; xrv2=0.;
            switch (p1) 
	    {
	    case 3:
	      xrv0=(UDERS(1,0)*CP(span-p1+0,0)+
		    UDERS(1,1)*CP(span-p1+1,0)+
		    UDERS(1,2)*CP(span-p1+2,0)+
		    UDERS(1,3)*CP(span-p1+3,0));
	      xrv1=(UDERS(1,0)*CP(span-p1+0,1)+
		    UDERS(1,1)*CP(span-p1+1,1)+
		    UDERS(1,2)*CP(span-p1+2,1)+
		    UDERS(1,3)*CP(span-p1+3,1));
	      xrv2=(UDERS(1,0)*CP(span-p1+0,2)+
		    UDERS(1,1)*CP(span-p1+1,2)+
		    UDERS(1,2)*CP(span-p1+2,2)+
		    UDERS(1,3)*CP(span-p1+3,2));
	      break;
	    default:
	    {
	      for( int ii=0; ii<=p1; ii++ )
	      {
		xrv0+=UDERS(1,ii)*CP(span-p1+ii,0);
		xrv1+=UDERS(1,ii)*CP(span-p1+ii,1);
		xrv2+=UDERS(1,ii)*CP(span-p1+ii,2);
	      }
	    }
	    }
	    XR(i,0,0)=(xrv0 - xv0*xrv2)/xv2;  
	    XR(i,1,0)=(xrv1 - xv1*xrv2)/xv2;  
	  }
	  else
	  {
	    xrv0=0.; xrv1=0.; 
	    for( int ii=0; ii<=p1; ii++ )
	    {
	      xrv0+=UDERS(1,ii)*CP(span-p1+ii,0);
	      xrv1+=UDERS(1,ii)*CP(span-p1+ii,1);
	    }
	    XR(i,0,0)=xrv0;
	    XR(i,1,0)=xrv1;
	  }
	  
	}  
	else if( rangeDimension==3 )
	{
          if( nonUniformWeights )
	  {
	    xrv0=0.; xrv1=0.; xrv2=0.; xrv3=0.;
            switch (p1) 
	    {
	    case 3:
	      xrv0=(UDERS(1,0)*CP(span-p1+0,0)+
		    UDERS(1,1)*CP(span-p1+1,0)+
		    UDERS(1,2)*CP(span-p1+2,0)+
		    UDERS(1,3)*CP(span-p1+3,0));
	      xrv1=(UDERS(1,0)*CP(span-p1+0,1)+
		    UDERS(1,1)*CP(span-p1+1,1)+
		    UDERS(1,2)*CP(span-p1+2,1)+
		    UDERS(1,3)*CP(span-p1+3,1));
	      xrv2=(UDERS(1,0)*CP(span-p1+0,2)+
		    UDERS(1,1)*CP(span-p1+1,2)+
		    UDERS(1,2)*CP(span-p1+2,2)+
		    UDERS(1,3)*CP(span-p1+3,2));
	      xrv3=(UDERS(1,0)*CP(span-p1+0,3)+
		    UDERS(1,1)*CP(span-p1+1,3)+
		    UDERS(1,2)*CP(span-p1+2,3)+
		    UDERS(1,3)*CP(span-p1+3,3));
	      break;
	    default:
	    {
	      for( int ii=0; ii<=p1; ii++ )
	      {
		xrv0+=UDERS(1,ii)*CP(span-p1+ii,0);
		xrv1+=UDERS(1,ii)*CP(span-p1+ii,1);
		xrv2+=UDERS(1,ii)*CP(span-p1+ii,2);
		xrv3+=UDERS(1,ii)*CP(span-p1+ii,3);
	      }
	    }
	    }
	    XR(i,0,0)=(xrv0 - xv0*xrv3)/xv3;  
	    XR(i,1,0)=(xrv1 - xv1*xrv3)/xv3;  
	    XR(i,2,0)=(xrv2 - xv2*xrv3)/xv3;  
	  }
	  else
	  {
	    xrv0=0.; xrv1=0.; xrv2=0.; 
	    for( int ii=0; ii<=p1; ii++ )
	    {
	      xrv0+=UDERS(1,ii)*CP(span-p1+ii,0);
	      xrv1+=UDERS(1,ii)*CP(span-p1+ii,1);
	      xrv2+=UDERS(1,ii)*CP(span-p1+ii,2);
	    }
	    XR(i,0,0)=xrv0;
	    XR(i,1,0)=xrv1;
	    XR(i,2,0)=xrv2;
	  }


// 	  xrv=0.;
// 	  for( int ii=0; ii<=p1; ii++ )
// 	    xrv(0,Rw)+=UDERS(1,ii)*cPoint(span-p1+ii,Rw);
// 	  if( nonUniformWeights )
// 	    xrv(0,R)=(xrv(0,R) - xv(0,R)*xrv(0,nw))*(1./xv(0,nw));  
// 	  xr(i,R,0)=xrv(0,R);
	}
        else if( rangeDimension==1 )
	{
          if( nonUniformWeights )
	  {
	    xrv0=0.; xrv1=0.; 
            switch (p1) 
	    {
	    case 3:
	      xrv0=(UDERS(1,0)*CP(span-p1+0,0)+
		    UDERS(1,1)*CP(span-p1+1,0)+
		    UDERS(1,2)*CP(span-p1+2,0)+
		    UDERS(1,3)*CP(span-p1+3,0));
	      xrv1=(UDERS(1,0)*CP(span-p1+0,1)+
		    UDERS(1,1)*CP(span-p1+1,1)+
		    UDERS(1,2)*CP(span-p1+2,1)+
		    UDERS(1,3)*CP(span-p1+3,1));
	      break;
	    default:
	    {
	      for( int ii=0; ii<=p1; ii++ )
	      {
		xrv0+=UDERS(1,ii)*CP(span-p1+ii,0);
		xrv1+=UDERS(1,ii)*CP(span-p1+ii,1);
	      }
	    }
	    }
	    XR(i,0,0)=(xrv0 - xv0*xrv1)/xv1;  
	  }
	  else
	  {
	    xrv0=0.; 
	    for( int ii=0; ii<=p1; ii++ )
	    {
	      xrv0+=UDERS(1,ii)*CP(span-p1+ii,0);
	    }
	    XR(i,0,0)=xrv0;
	  }
	  
	}  
	else
	{
	  printf("NurbsMapping::map:ERROR: rangeDimension=%i\n",rangeDimension);
	  throw "error";
	}

      }
    }
  }
  else if( domainDimension==2 )
  {
    int order= computeMapDerivative ? 1 : 0;  // compute derivatives up to this order
    int uSpan,vSpan;
    real r0Previous=100., r1Previous=100.;
    rScale[0]=rEnd[axis1]-rStart[axis1];
    rScale[1]=rEnd[axis2]-rStart[axis2];
    reScale[0] =rScale[0]!=1. || rStart[0]!=0.;
    reScale[1]=rScale[1]!=1. || rStart[1]!=0.;

    real vdersp[2*maximumOrder];
//    real *vdersp = vDers.getDataPointer();
    const int ndcp2 = ndcp*cPoint.getLength(1);
    const int p2p=p2+1;
    real uTempp[4*maximumOrder];
    
#undef CP
#define CP(i,j,k) cPointp[(i)+ndcp*(j)+ndcp2*(k)]
#define VDERS(i,j) vdersp[(i)+2*(j)]
#define UTEMP(i,j) uTempp[(j)+4*(i)]
   
    if( rangeDimension==3 )
    {
      for( int i=base; i<=bound; i++ )
      {
	r0=RR(i,0);
	r1=RR(i,1);

	if( r0!= r0Previous )
	{
	  uSpan = findSpan( n1,p1,r0,uKnot );
	  dersBasisFuns(uSpan,r0,p1,order,uKnot,udersp); 
	  r0Previous=r0;
	}
	if( r1!=r1Previous )
	{
	  vSpan = findSpan( n2,p2,r1,vKnot );
	  dersBasisFuns(vSpan,r1,p2,order,vKnot,vdersp); 
	  r1Previous=r1;
	}

	int cmd = computeMapDerivative? 1 : 0;
	for( int ud=0; ud<=cmd; ud++ )
	{
	  int i2;
	  for( i2=0; i2<=p2; i2++ )
	  {
	    if( p1==3 )
	    { // common case
	      UTEMP(i2,0)=(UDERS(ud,0)*CP(uSpan-p1+0,vSpan-p2+i2,0)+
			   UDERS(ud,1)*CP(uSpan-p1+1,vSpan-p2+i2,0)+
			   UDERS(ud,2)*CP(uSpan-p1+2,vSpan-p2+i2,0)+
			   UDERS(ud,3)*CP(uSpan-p1+3,vSpan-p2+i2,0));
	      UTEMP(i2,1)=(UDERS(ud,0)*CP(uSpan-p1+0,vSpan-p2+i2,1)+
			   UDERS(ud,1)*CP(uSpan-p1+1,vSpan-p2+i2,1)+
			   UDERS(ud,2)*CP(uSpan-p1+2,vSpan-p2+i2,1)+
			   UDERS(ud,3)*CP(uSpan-p1+3,vSpan-p2+i2,1));
	      UTEMP(i2,2)=(UDERS(ud,0)*CP(uSpan-p1+0,vSpan-p2+i2,2)+
			   UDERS(ud,1)*CP(uSpan-p1+1,vSpan-p2+i2,2)+
			   UDERS(ud,2)*CP(uSpan-p1+2,vSpan-p2+i2,2)+
			   UDERS(ud,3)*CP(uSpan-p1+3,vSpan-p2+i2,2));
	      if( nonUniformWeights )
		UTEMP(i2,3)=(UDERS(ud,0)*CP(uSpan-p1+0,vSpan-p2+i2,3)+
			     UDERS(ud,1)*CP(uSpan-p1+1,vSpan-p2+i2,3)+
			     UDERS(ud,2)*CP(uSpan-p1+2,vSpan-p2+i2,3)+
			     UDERS(ud,3)*CP(uSpan-p1+3,vSpan-p2+i2,3));
	    }
	    else
	    {
	      UTEMP(i2,0)=0.; UTEMP(i2,1)=0.; UTEMP(i2,2)=0.; 
	      if( nonUniformWeights ) UTEMP(i2,3)=0.;
	      for( int i1=0; i1<=p1; i1++ )
	      {
		UTEMP(i2,0)+=UDERS(ud,i1)*CP(uSpan-p1+i1,vSpan-p2+i2,0);
		UTEMP(i2,1)+=UDERS(ud,i1)*CP(uSpan-p1+i1,vSpan-p2+i2,1);
		UTEMP(i2,2)+=UDERS(ud,i1)*CP(uSpan-p1+i1,vSpan-p2+i2,2);
		if( nonUniformWeights )
		  UTEMP(i2,3)+=UDERS(ud,i1)*CP(uSpan-p1+i1,vSpan-p2+i2,3);
	      }
	    
	    }
	  }
	  for( int vd=0; vd<=cmd-ud; vd++ )
	  {
	    if( p2==3 )
	    {// common case
	      xrv0=VDERS(vd,0)*UTEMP(0,0)+VDERS(vd,1)*UTEMP(1,0)+VDERS(vd,2)*UTEMP(2,0)+VDERS(vd,3)*UTEMP(3,0);
	      xrv1=VDERS(vd,0)*UTEMP(0,1)+VDERS(vd,1)*UTEMP(1,1)+VDERS(vd,2)*UTEMP(2,1)+VDERS(vd,3)*UTEMP(3,1);
	      xrv2=VDERS(vd,0)*UTEMP(0,2)+VDERS(vd,1)*UTEMP(1,2)+VDERS(vd,2)*UTEMP(2,2)+VDERS(vd,3)*UTEMP(3,2);
	      if( nonUniformWeights )
		xrv3=VDERS(vd,0)*UTEMP(0,3)+VDERS(vd,1)*UTEMP(1,3)+VDERS(vd,2)*UTEMP(2,3)+VDERS(vd,3)*UTEMP(3,3);
	    }
	    else
	    {
	      xrv0=0., xrv1=0., xrv2=0., xrv3=0.;
	      for( i2=0; i2<=p2; i2++ )
	      {
		xrv0+=VDERS(vd,i2)*UTEMP(i2,0);
		xrv1+=VDERS(vd,i2)*UTEMP(i2,1);
		xrv2+=VDERS(vd,i2)*UTEMP(i2,2);
		if( nonUniformWeights ) xrv3+=VDERS(vd,i2)*UTEMP(i2,3);
	      }
	    }
	    if( ud==0 && vd==0 )
	    {
	      xv0=xrv0;  // save for derivative with non-uniform weights
	      xv1=xrv1;  
	      xv2=xrv2;  
	      if( nonUniformWeights )
	      {
		xv3=1./xrv3;  
		xv0*=xv3;
		xv1*=xv3;
		xv2*=xv3;
	      }
	      if( computeMap )
	      {
		X(i,0)=xv0;
		X(i,1)=xv1;
		X(i,2)=xv2;
	      }
	    
	    }
	    else
	    {
	      if( nonUniformWeights )
	      {
		XR(i,0,vd)=(xrv0 - xv0*xrv3)*xv3;
		XR(i,1,vd)=(xrv1 - xv1*xrv3)*xv3;
		XR(i,2,vd)=(xrv2 - xv2*xrv3)*xv3;
	      }
	      else
	      {
		XR(i,0,vd)=xrv0;
		XR(i,1,vd)=xrv1;
		XR(i,2,vd)=xrv2;
	      }
	    }
	    
	  }
	}
      }
    }
    else if( rangeDimension==2 )
    {
      // *wdh* added 031211
      for( int i=base; i<=bound; i++ )
      {
	r0=RR(i,0);
	r1=RR(i,1);

	if( r0!= r0Previous )
	{
	  uSpan = findSpan( n1,p1,r0,uKnot );
	  dersBasisFuns(uSpan,r0,p1,order,uKnot,udersp); 
	  r0Previous=r0;
	}
	if( r1!=r1Previous )
	{
	  vSpan = findSpan( n2,p2,r1,vKnot );
	  dersBasisFuns(vSpan,r1,p2,order,vKnot,vdersp); 
	  r1Previous=r1;
	}

	int cmd = computeMapDerivative? 1 : 0;
	for( int ud=0; ud<=cmd; ud++ )
	{
	  int i2;
	  for( i2=0; i2<=p2; i2++ )
	  {
	    if( p1==3 )
	    { // common case
	      UTEMP(i2,0)=(UDERS(ud,0)*CP(uSpan-p1+0,vSpan-p2+i2,0)+
			   UDERS(ud,1)*CP(uSpan-p1+1,vSpan-p2+i2,0)+
			   UDERS(ud,2)*CP(uSpan-p1+2,vSpan-p2+i2,0)+
			   UDERS(ud,3)*CP(uSpan-p1+3,vSpan-p2+i2,0));
	      UTEMP(i2,1)=(UDERS(ud,0)*CP(uSpan-p1+0,vSpan-p2+i2,1)+
			   UDERS(ud,1)*CP(uSpan-p1+1,vSpan-p2+i2,1)+
			   UDERS(ud,2)*CP(uSpan-p1+2,vSpan-p2+i2,1)+
			   UDERS(ud,3)*CP(uSpan-p1+3,vSpan-p2+i2,1));
	      if( nonUniformWeights )
		UTEMP(i2,2)=(UDERS(ud,0)*CP(uSpan-p1+0,vSpan-p2+i2,3)+
			     UDERS(ud,1)*CP(uSpan-p1+1,vSpan-p2+i2,3)+
			     UDERS(ud,2)*CP(uSpan-p1+2,vSpan-p2+i2,3)+
			     UDERS(ud,3)*CP(uSpan-p1+3,vSpan-p2+i2,3));
	    }
	    else
	    {
	      UTEMP(i2,0)=0.; UTEMP(i2,1)=0.; 
	      if( nonUniformWeights ) UTEMP(i2,2)=0.;
	      for( int i1=0; i1<=p1; i1++ )
	      {
		UTEMP(i2,0)+=UDERS(ud,i1)*CP(uSpan-p1+i1,vSpan-p2+i2,0);
		UTEMP(i2,1)+=UDERS(ud,i1)*CP(uSpan-p1+i1,vSpan-p2+i2,1);
		if( nonUniformWeights )
		  UTEMP(i2,2)+=UDERS(ud,i1)*CP(uSpan-p1+i1,vSpan-p2+i2,3);
	      }
	    
	    }
	  }
	  for( int vd=0; vd<=cmd-ud; vd++ )
	  {
	    if( p2==3 )
	    {// common case
	      xrv0=VDERS(vd,0)*UTEMP(0,0)+VDERS(vd,1)*UTEMP(1,0)+VDERS(vd,2)*UTEMP(2,0)+VDERS(vd,3)*UTEMP(3,0);
	      xrv1=VDERS(vd,0)*UTEMP(0,1)+VDERS(vd,1)*UTEMP(1,1)+VDERS(vd,2)*UTEMP(2,1)+VDERS(vd,3)*UTEMP(3,1);
	      if( nonUniformWeights )
		xrv3=VDERS(vd,0)*UTEMP(0,2)+VDERS(vd,1)*UTEMP(1,2)+VDERS(vd,2)*UTEMP(2,2)+VDERS(vd,3)*UTEMP(3,2);
	    }
	    else
	    {
	      xrv0=0., xrv1=0., xrv3=0.;
	      for( i2=0; i2<=p2; i2++ )
	      {
		xrv0+=VDERS(vd,i2)*UTEMP(i2,0);
		xrv1+=VDERS(vd,i2)*UTEMP(i2,1);
		if( nonUniformWeights ) xrv3+=VDERS(vd,i2)*UTEMP(i2,2);
	      }
	    }
	    if( ud==0 && vd==0 )
	    {
	      xv0=xrv0;  // save for derivative with non-uniform weights
	      xv1=xrv1;  
	      if( nonUniformWeights )
	      {
		xv3=1./xrv3;  
		xv0*=xv3;
		xv1*=xv3;
	      }
	      if( computeMap )
	      {
		X(i,0)=xv0;
		X(i,1)=xv1;
	      }
	    
	    }
	    else
	    {
	      if( nonUniformWeights )
	      {
		XR(i,0,vd)=(xrv0 - xv0*xrv3)*xv3;
		XR(i,1,vd)=(xrv1 - xv1*xrv3)*xv3;
	      }
	      else
	      {
		XR(i,0,vd)=xrv0;
		XR(i,1,vd)=xrv1;
	      }
	    }
	    
	  }
	}
      }



    }


    else
    {
      Overture::abort("error");
    }
  } // end domainDimension==2
  else if( domainDimension==3 )
  {
    // printf("Evaluate Nurbs using new 3d\n");

    int order= computeMapDerivative ? 1 : 0;  // compute derivatives up to this order
    int uSpan,vSpan,wSpan;
    real r0Previous=100., r1Previous=100., r2Previous=100.;
    rScale[0]=rEnd[axis1]-rStart[axis1];
    rScale[1]=rEnd[axis2]-rStart[axis2];
    rScale[2]=rEnd[axis3]-rStart[axis3];
    reScale[0]=rScale[0]!=1. || rStart[0]!=0.;
    reScale[1]=rScale[1]!=1. || rStart[1]!=0.;
    reScale[2]=rScale[2]!=1. || rStart[2]!=0.;

    real vdersp[2*maximumOrder];
    real wdersp[2*maximumOrder];
//    real *vdersp = vDers.getDataPointer();
    const int ndcp2 = ndcp*cPoint.getLength(1); 
    const int ndcp3 = ndcp2*cPoint.getLength(2);
    const int p2p=p2+1;
    real uTempp[4*maximumOrder*maximumOrder];
    real vTempp[4*maximumOrder];
    
#undef CP
#undef VDERS
#undef UTEMP
#undef VTEMP
#define CP(i1,i2,i3,i4) cPointp[(i1)+ndcp*(i2)+ndcp2*(i3)+ndcp3*(i4)]
#define VDERS(i,j) vdersp[(i)+2*(j)]
#define WDERS(i,j) wdersp[(i)+2*(j)]
#define UTEMP(i2,i3,j) uTempp[(j)+4*(i2+p2p*(i3))] 
#define VTEMP(i3,j) vTempp[(j)+4*(i3)] 

    real r2;
    for( int i=base; i<=bound; i++ )
    {
      r0=RR(i,0);
      r1=RR(i,1);
      r2=RR(i,2);

      if( r0!= r0Previous )
      {
	uSpan = findSpan( n1,p1,r0,uKnot );
	dersBasisFuns(uSpan,r0,p1,order,uKnot,udersp); 
	r0Previous=r0;
      }
      if( r1!=r1Previous )
      {
	vSpan = findSpan( n2,p2,r1,vKnot );
	dersBasisFuns(vSpan,r1,p2,order,vKnot,vdersp); 
	r1Previous=r1;
      }
      if( r2!=r2Previous )
      {
	wSpan = findSpan( n3,p3,r2,wKnot );
	dersBasisFuns(wSpan,r2,p3,order,wKnot,wdersp); 
	r2Previous=r2;
      }

      int cmd = computeMapDerivative? 1 : 0;
      for( int ud=0; ud<=cmd; ud++ )
      {
	int i2,i3;
	for( i3=0; i3<=p3; i3++ )
	for( i2=0; i2<=p2; i2++ )
	{
          const int j2=vSpan-p2+i2, j3=wSpan-p3+i3;
	  if( p1==3 )
	  { // common case
	    UTEMP(i2,i3,0)=(UDERS(ud,0)*CP(uSpan-p1+0,j2,j3,0)+
			    UDERS(ud,1)*CP(uSpan-p1+1,j2,j3,0)+
			    UDERS(ud,2)*CP(uSpan-p1+2,j2,j3,0)+
			    UDERS(ud,3)*CP(uSpan-p1+3,j2,j3,0));
	    UTEMP(i2,i3,1)=(UDERS(ud,0)*CP(uSpan-p1+0,j2,j3,1)+
			    UDERS(ud,1)*CP(uSpan-p1+1,j2,j3,1)+
			    UDERS(ud,2)*CP(uSpan-p1+2,j2,j3,1)+
			    UDERS(ud,3)*CP(uSpan-p1+3,j2,j3,1));
	    UTEMP(i2,i3,2)=(UDERS(ud,0)*CP(uSpan-p1+0,j2,j3,2)+
			    UDERS(ud,1)*CP(uSpan-p1+1,j2,j3,2)+
			    UDERS(ud,2)*CP(uSpan-p1+2,j2,j3,2)+
			    UDERS(ud,3)*CP(uSpan-p1+3,j2,j3,2));
	    if( nonUniformWeights )
	      UTEMP(i2,i3,3)=(UDERS(ud,0)*CP(uSpan-p1+0,j2,j3,3)+
			      UDERS(ud,1)*CP(uSpan-p1+1,j2,j3,3)+
			      UDERS(ud,2)*CP(uSpan-p1+2,j2,j3,3)+
			      UDERS(ud,3)*CP(uSpan-p1+3,j2,j3,3));
	  }
	  else
	  {
	    UTEMP(i2,i3,0)=0.; UTEMP(i2,i3,1)=0.; UTEMP(i2,i3,2)=0.; 
	    if( nonUniformWeights ) UTEMP(i2,i3,3)=0.;
	    for( int i1=0; i1<=p1; i1++ )
	    {
              const int j1=uSpan-p1+i1;
	      UTEMP(i2,i3,0)+=UDERS(ud,i1)*CP(j1,j2,j3,0);
	      UTEMP(i2,i3,1)+=UDERS(ud,i1)*CP(j1,j2,j3,1);
	      UTEMP(i2,i3,2)+=UDERS(ud,i1)*CP(j1,j2,j3,2);
	      if( nonUniformWeights )
		UTEMP(i2,i3,3)+=UDERS(ud,i1)*CP(j1,j2,j3,3);
	    }
	    
	  }
	}
	for( int vd=0; vd<=cmd-ud; vd++ )
	{
	  if( p2==3 )
	  { // common case
	    for( i3=0; i3<=p3; i3++ )
	    {
	      VTEMP(i3,0)=(VDERS(vd,0)*UTEMP(0,i3,0)+
			   VDERS(vd,1)*UTEMP(1,i3,0)+
			   VDERS(vd,2)*UTEMP(2,i3,0)+
			   VDERS(vd,3)*UTEMP(3,i3,0));
	      VTEMP(i3,1)=(VDERS(vd,0)*UTEMP(0,i3,1)+
			   VDERS(vd,1)*UTEMP(1,i3,1)+
			   VDERS(vd,2)*UTEMP(2,i3,1)+
			   VDERS(vd,3)*UTEMP(3,i3,1));
	      VTEMP(i3,2)=(VDERS(vd,0)*UTEMP(0,i3,2)+
			   VDERS(vd,1)*UTEMP(1,i3,2)+
			   VDERS(vd,2)*UTEMP(2,i3,2)+
			   VDERS(vd,3)*UTEMP(3,i3,2));
	      if( nonUniformWeights )
		VTEMP(i3,3)=(VDERS(vd,0)*UTEMP(0,i3,3)+
			     VDERS(vd,1)*UTEMP(1,i3,3)+
			     VDERS(vd,2)*UTEMP(2,i3,3)+
			     VDERS(vd,3)*UTEMP(3,i3,3));
	    }
	  
	  }
	  else
	  {
	    for( i3=0; i3<=p3; i3++ )
	    {
	      VTEMP(i3,0)=0.; VTEMP(i3,1)=0.; VTEMP(i3,2)=0.; 
	      if( nonUniformWeights ) VTEMP(i3,3)=0.;
	      for( int i2=0; i2<=p2; i2++ )
	      {
		VTEMP(i3,0)+=VDERS(vd,i2)*UTEMP(i2,i3,0); 
		VTEMP(i3,1)+=VDERS(vd,i2)*UTEMP(i2,i3,1); 
		VTEMP(i3,2)+=VDERS(vd,i2)*UTEMP(i2,i3,2);
		if( nonUniformWeights )
		  VTEMP(i3,3)+=VDERS(vd,i2)*UTEMP(i2,i3,3); 
	      }
	      
	    }
	  }
	  

	  for( int wd=0; wd<=cmd-max(ud,vd); wd++ )
	  {
	    if( p3==3 )
	    {// common case
	      xrv0=WDERS(wd,0)*VTEMP(0,0)+WDERS(wd,1)*VTEMP(1,0)+WDERS(wd,2)*VTEMP(2,0)+WDERS(wd,3)*VTEMP(3,0);
	      xrv1=WDERS(wd,0)*VTEMP(0,1)+WDERS(wd,1)*VTEMP(1,1)+WDERS(wd,2)*VTEMP(2,1)+WDERS(wd,3)*VTEMP(3,1);
	      xrv2=WDERS(wd,0)*VTEMP(0,2)+WDERS(wd,1)*VTEMP(1,2)+WDERS(wd,2)*VTEMP(2,2)+WDERS(wd,3)*VTEMP(3,2);
	      if( nonUniformWeights )
		xrv3=WDERS(wd,0)*VTEMP(0,3)+WDERS(wd,1)*VTEMP(1,3)+WDERS(wd,2)*VTEMP(2,3)+WDERS(wd,3)*VTEMP(3,3);
	    }
	    else
	    {
	      xrv0=0., xrv1=0., xrv2=0., xrv3=0.;
	      for( i3=0; i3<=p3; i3++ )
	      {
		xrv0+=WDERS(wd,i3)*VTEMP(i3,0);
		xrv1+=WDERS(wd,i3)*VTEMP(i3,1);
		xrv2+=WDERS(wd,i3)*VTEMP(i3,2);
		if( nonUniformWeights ) xrv3+=WDERS(wd,i3)*VTEMP(i3,3);
	      }
	    }

	    if( ud==0 && vd==0 && wd==0 )
	    {
	      xv0=xrv0;  // save for derivative with non-uniform weights
	      xv1=xrv1;  
	      xv2=xrv2;  
	      if( nonUniformWeights )
	      {
		xv3=1./xrv3;  
		xv0*=xv3;
		xv1*=xv3;
		xv2*=xv3;
	      }
	      if( computeMap )
	      {
		X(i,0)=xv0;
		X(i,1)=xv1;
		X(i,2)=xv2;
	      }
	    
	    }
	    else
	    {
	      // (ud,vd,wd) = (1,0,0) : n=0 : d/dr
	      // (ud,vd,wd) = (0,1,0) : n=1 : d/ds
	      // (ud,vd,wd) = (0,0,1) : n=2 : d/dt
	      const int n = vd+2*wd; 
	      assert( (ud+vd+wd)==1 && n>=0 && n<=2 );
	      if( nonUniformWeights )
	      {
		XR(i,0,n)=(xrv0 - xv0*xrv3)*xv3;
		XR(i,1,n)=(xrv1 - xv1*xrv3)*xv3;
		XR(i,2,n)=(xrv2 - xv2*xrv3)*xv3;
	      }
	      else
	      {
		XR(i,0,n)=xrv0;
		XR(i,1,n)=xrv1;
		XR(i,2,n)=xrv2;
	      }
	    }
	  }
	  
	}
      }
    }
  } // end domainDimension==3
  
  if( computeMapDerivative )
  {
    for( axis=0; axis<domainDimension; axis++ )
    {
      if( reScale[axis] )
      {
        // xr_(I,R,axis)*=rScale[axis];
 	for( int dir=0; dir<rangeDimension; dir++ )
 	  for( int i=base; i<=bound; i++ )
 	    XR(i,dir,axis)*=rScale[axis];
      }
      
    }
  }

  nurbTimeEvaluate+=getCPU()-time0;
  
}
#undef UDERS
#undef CP
#undef R
#undef RR
#undef X
#undef XR


real nurbApproximateInverseTime=0.;
real nurbApproximateInverseTimeNew=0.;

void NurbsMapping::
basicInverse( const realArray & x,
	      realArray & r, 
	      realArray & rx /* = Overture::nullRealDistributedArray() */,
	      MappingParameters & params /* =Overture::nullMappingParameters() */ )
// ==========================================================================================
// /Description:
//     Optimized inverse.
// ==========================================================================================
{
  // *** this routine fails with topoElectrodeInner.bug.cmd
  #ifndef USE_PPP
    basicInverse(x,r,rx,params);
  #else
    realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(x,xLocal);
    realSerialArray rLocal;  getLocalArrayWithGhostBoundaries(r,rLocal);
    realSerialArray rxLocal; getLocalArrayWithGhostBoundaries(rx,rxLocal);
    basicInverseS(xLocal,rLocal,rxLocal,params);
  #endif
}



void NurbsMapping::
basicInverseS( const RealArray & x,
	       RealArray & r, 
	       RealArray & rx /* = Overture::nullRealDistributedArray() */,
	       MappingParameters & params /* =Overture::nullMappingParameters() */ )
// ==========================================================================================
// /Description:
//     Optimized inverse.
// ==========================================================================================
{
  // *** this routine fails with topoElectrodeInner.bug.cmd

  if( true )
  {
    printf(" NurbsMapping::basicInverse called *****\n");
    
    setBasicInverseOption(canDoNothing);  

    MappingWorkSpace workSpace; 
    if( params.computeGlobalInverse )
    {
      // first get the initial guess
      approximateGlobalInverse->inverse( x,r,rx,workSpace,params );

    // Now do Newton to Invert:
      exactLocalInverse->inverse( x,r,rx,workSpace,TRUE );   // TRUE means use results found in the
      // workSpace
    }
    else
      exactLocalInverse->inverse( x,r,rx,workSpace,FALSE );

    setBasicInverseOption(canInvert);  
    
    return;
  }
  
// *   // first get the initial guess
// *   setBasicInverseOption(canDoNothing);  
// *   real time0=getCPU();
// *   MappingWorkSpace workSpace; 
// *   if( false && domainDimension==1 )
// *   {
// *     approximateGlobalInverse->inverse( x,r,rx,workSpace,params );
// *     // another way:
// *     real time1=getCPU();
// *     const realArray & g = getGrid();
// *     const int n= g.getLength(0);
// *     real *gp = g.getDataPointer();
// * #define G(i,j) gp[(i)+n*(j)]
// * 
// *     base = x.getBase(0), bound =x.getBound(0);
// *     if( rangeDimension==2 )
// *     {
// *       real xx[2];
// *       real distMin=REAL_MAX;
// *       int jMin=-1;
// *       for( int i=base; i<=bound; i++ )
// *       {
// * 	xx[0]=x(i,0), xx[1]=x(i,1);
// * 	
// *         for( int j=0; j<n; j++ )
// * 	{
// * 	  real dist = fabs(xx[0]-G(j,0))+fabs(xx[1]-G(j,1));
// * 	  
// *           if( dist<distMin )
// * 	  {
// * 	    jMin=j;
// * 	  }
// * 	}
// * 	if( computeMap )
// * 	  r(i,0)=jMin/(n-1.);
// *       }  // end for i
// *     }
// *     nurbApproximateInverseTimeNew=getCPU()-time1;
// * #undef G
// *   }
// *   else
// *   {
// *     if( params.computeGlobalInverse )
// *       approximateGlobalInverse->inverse( x,r,rx,workSpace,params );
// *   }
// *   nurbApproximateInverseTime=getCPU()-time0;
// * 
// *   // Now do Newton to Invert:
// *   // *wdh* exactLocalInverse->inverse( x,r,rx,workSpace,TRUE );
// *   if( params.computeGlobalInverse )
// *     exactLocalInverse->inverse( x,r,rx,workSpace,TRUE );
// *   else
// *     exactLocalInverse->inverse( x,r,rx,workSpace,FALSE );
// *   
// *   setBasicInverseOption(canInvert);  

}







// ================= vectorized versions ===========================

void NurbsMapping::
findSpan(const int & n ,
         const int & p,
         const Index & I,
         const RealArray & u,
         const RealArray & knot,
         IntegerArray & span )
//
// Determine the knot span index
//
// Assume the knot is clamped (nonperiodic or open):
//   knot = { u_0,u_0,...,u_0, u_1, ..., u_n, u_{n+1}, u_{n+1} }
//              0   1  ... p    p+1 ...   m-p-1 ...      m
//
// /n,p (input) : n=m-p-1, and p=degree of the nurbs
// /u (input) : find index with knot(index) <= u < knot(index+1)
{
  int low =p, high=n+1, mid=(low+high)/2;
  int ub=u.getBase(1);
  
  for( int i=I.getBase(); i<=I.getBound(); i++ )
  {
    real uu = u(i,ub);
    if( uu >= knot(n+1) )
      span(i)=n;
    else if( uu <=knot(p) )
      span(i)=p;
    else if( uu>=knot(mid) && uu<knot(mid+1) )
      span(i)=mid;  // check for the same interval as last time
    else
    {
      // binary search:
      low =p, high=n+1, mid=(low+high)/2;
      while( uu<knot(mid) || uu>=knot(mid+1) )
      {
	if( uu<knot(mid) )
	  high=mid;
	else
	  low=mid;
	mid=(low+high)/2;
      }
      span(i)=mid;
    }
    
  }
}


void NurbsMapping::
dersBasisFuns(const Index & I,
	      const IntegerArray & ia,
	      const RealArray & u,
	      const int & p,
	      const int & order,
	      const RealArray & knot,
	      RealArray & ders )
//
//  Compute the non-vanishing basis functions and their derivatives up to order d
// /i,u,p,knot (input):
// /order (input) order of derivative ( order<=p )
// /ders(0:d,0:p) : 
//
{
  int j,k,j1,j2,r;
  int iStart=I.getBase(), iEnd=I.getBound();
  int ub=u.getBase(1);
  
  
  RealArray leftV(I,p+1), rightV(I,p+1), temp1(I), temp2(I), nduV(I,p+1,p+1);

  nduV(I,0,0)=1.;
  for( j=1; j<=p; j++ )
  {
    for( int i=iStart; i<=iEnd; i++ )
    {
      leftV(i,j)=u(i,ub)-knot(ia(i)+1-j);
      rightV(i,j)=knot(ia(i)+j)-u(i,ub);
    }
    
    temp1(I)=0.;
    for( r=0; r<j; r++ )
    {
      nduV(I,j,r)=rightV(I,r+1)+leftV(I,j-r);
      temp2(I)=nduV(I,r,j-1)/nduV(I,j,r);
      
      nduV(I,r,j)=temp1(I)+rightV(I,r+1)*temp2(I);
      temp1(I)=leftV(I,j-r)*temp2(I);
    }
    nduV(I,j,j)=temp1(I);
  }
  for( j=0; j<=p; j++ )
    ders(I,0,j)=nduV(I,j,p);  // basis function
    
  if( order==0 )
    return;
  
  // compute derivatives
  RealArray aV(I,2,p+1);
  
  for( r=0; r<=p; r++ )
  {
    int s1=0, s2=1;  // alternate rows in a
    aV(I,0,0)=1.;
    // compute k'th derivative
    for( k=1; k<=order; k++ )
    {
      temp2(I)=0.;
      int rk=r-k, pk=p-k;
      if( r>=k )
      {
	aV(I,s2,0)=aV(I,s1,0)/nduV(I,pk+1,rk);
	temp2(I)=aV(I,s2,0)*nduV(I,rk,pk);
      }
      j1 = rk >= -1 ? 1 : -rk;
      j2 = (r-1<=pk) ? k-1 : p-r;
      for( j=j1; j<=j2; j++ )
      {
	aV(I,s2,j)=(aV(I,s1,j)-aV(I,s1,j-1))/nduV(I,pk+1,rk+j);
	temp2(I)+=aV(I,s2,j)*nduV(I,rk+j,pk);
      }
      if( r<=pk )
      {
	aV(I,s2,k)= -aV(I,s1,k-1)/nduV(I,pk+1,r);
	temp2(I)+=aV(I,s2,k)*nduV(I,r,pk);
      }
      ders(I,k,r)=temp2(I);
      j=s1; s1=s2; s2=j;  // switch rows
    }
  }
  r=p;
  for( k=1; k<=order; k++ )
  {
    for( j=0; j<=p; j++ ) 
      ders(I,k,j)*=r;
    r*=p-k;
  }
    
}


#define ForR(m)  for( m=0; m<rangeDimension; m++ )
#define ForRw(m) for( m=0; m<=Rw.getBound(); m++ )

#define ForI(i)  for( i=I.getBase(); i<=I.getBound(); i++ )

void NurbsMapping::
mapVector( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
//=====================================================================================
/// \brief  Evaluate the nurbs and/or derivatives. This routine is a
///   version of the {\tt map} function that is optimized for vectors of points.
//=====================================================================================
{

  if( params.coordinateType != cartesian )
    cerr << "NurbsMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  // cout << "NurbsMapping:mapVector: number of points = " << bound-base+1 << endl;

  int m,i;
  int nw=rangeDimension;  // position of the weight in the cPoint array
  Range R(0,rangeDimension-1);
  // control points includes the weights as a last component, if the weights are nonuniform
  // then we must compute the b-spline for the weight component too:
  Range Rw= nonUniformWeights ? Range(0,rangeDimension) : R;
  

  const int p =max(p1,p2);
  
//   // for dersBasisFuns
//   if( base<uIndex.getBase(0) || bound > uIndex.getBound(0) || leftV.getBase(1)!=p+1 )
//   {
//     Range R=I;
//     leftV.redim(R,p+1);    // *********************** remove these from the class, just build local copies *****
//     rightV.redim(R,p+1);
//     temp1.redim(R);
//     temp2.redim(R);
//     nduV.redim(R,p+1,p+1);
//     uIndex.redim(R);
//     uDersV.redim(R,2,p1+1);
//     xV.redim(R,Rw);

//     if( domainDimension>1 )
//     {
//       vIndex.redim(R);
//       vDersV.redim(R,2,p2+1);
//       uTempV.redim(R,p2+1,4);
//     }
//     xrV.redim(R,Rw);
//   }

//   if( computeMapDerivative && (base<aV.getBase(0) || bound > aV.getBound(0)) )
//   {
//     aV.redim(Range(I),2,p+1);
//   }
  
  real rScale[3];
  bool reScale[3];
  RealArray rr(I,domainDimension);
  int axis;
  for( axis=0; axis<domainDimension; axis++ )
  {
    rScale[axis]=rEnd[axis]-rStart[axis];
    reScale[axis] = rScale[axis]!=1. || rStart[axis]!=0.;
    if( nurbsIsPeriodic[axis]==functionPeriodic || reScale[axis] )  // ****** we need to add shift for derivative periodic
    {
      // if( reScale[axis] )
      //   printf("NurbsMapping: axis=%i, rScale=%e nurbsIsPeriodic=%i\n",axis,rScale,nurbsIsPeriodic[axis]);
    
      if( !nurbsIsPeriodic[axis] )
	rr(I,axis)=rScale[axis]*r(I,axis)+rStart[axis];  // rescale non-periodic
      else
      {
	if( reScale[axis] )
	  rr(I,axis)=fmod(rScale[axis]*r(I,axis)+rStart[axis]+2.,1.);  // we enforce rStart>=-1.
	else
	  rr(I,axis)=fmod(r(I,axis)+1.,1.);   // map to [0,1] ** assumes r>=-1.
      }
    }
    else
      rr(I,axis)=r(I,axis);
  }

  IntegerArray uIndex(I);
  RealArray xV(I,Rw), uDersV(I,2,p1+1), xrV(I,Rw);

  if( domainDimension==1 )
  {
    int order= computeMapDerivative ? 1 : 0;  // compute derivatives up to this order

    findSpan( n1,p1,I,rr,uKnot,uIndex );

    dersBasisFuns(I,uIndex,rr,p1,order,uKnot,uDersV); 

    if( computeMap || nonUniformWeights )
    {
      xV(I,Rw)=0.;
      for( int ii=0; ii<=p1; ii++ )
	ForRw(m) xV(I,m)+=uDersV(I,0,ii)*cPoint(uIndex(I)-p1+ii,m);
      // ForRw(m) ForI(i) xV(i,m)+=uDersV(i,0,ii)*cPoint(uIndex(i)-p1+ii,m);
      if( nonUniformWeights )
	ForR(m) xV(I,m)/=xV(I,nw);
    }
    if( computeMap )
      x(I,R)=xV(I,R);

    if( computeMapDerivative )
    {
      xrV=0.;
      for( int ii=0; ii<=p1; ii++ )
	ForRw(m) xrV(I,m)+=uDersV(I,1,ii)*cPoint(uIndex(I)-p1+ii,m);
      //ForRw(m)  ForI(i) xrV(i,m)+=uDersV(i,1,ii)*cPoint(uIndex(i)-p1+ii,m);
      if( nonUniformWeights )
	ForR(m) xrV(I,m)=(xrV(I,m) - xV(I,m)*xrV(I,nw))/xV(I,nw);  
      xr(I,R,0)=xrV(I,R);
    }
  }
  else if( domainDimension==2 )
  {
    int order= computeMapDerivative ? 1 : 0;  // compute derivatives up to this order

    IntegerArray vIndex(I);
    RealArray vDersV(I,2,p2+1), uTempV(I,p2+1,Rw), xrV(I,Rw);
      
    findSpan( n1,p1,I,rr(I,0),uKnot,uIndex );
    findSpan( n2,p2,I,rr(I,1),vKnot,vIndex );

    dersBasisFuns(I,uIndex,rr(I,0),p1,order,uKnot,uDersV); 
    dersBasisFuns(I,vIndex,rr(I,1),p2,order,vKnot,vDersV); 
    

    int cmd = computeMapDerivative? 1 : 0;
    for( int ud=0; ud<=cmd; ud++ )
    {
      int i2;
      for( i2=0; i2<=p2; i2++ )
      {
	if( p1==3 )
	{ // common case
	  ForRw(m)
	  {
            ForI(i)
              uTempV(i,i2,m)=( uDersV(i,ud,0)*cPoint(uIndex(i)-p1+0,vIndex(i)-p2+i2,m)+
			       uDersV(i,ud,1)*cPoint(uIndex(i)-p1+1,vIndex(i)-p2+i2,m)+
			       uDersV(i,ud,2)*cPoint(uIndex(i)-p1+2,vIndex(i)-p2+i2,m)+
			       uDersV(i,ud,3)*cPoint(uIndex(i)-p1+3,vIndex(i)-p2+i2,m));
	  }
	}
	else
	{
	  ForRw(m) uTempV(I,i2,m)=0.; 
	  for( int i1=0; i1<=p1; i1++ )
	  {
	    ForRw(m) 
	    {
	      // uTempV(I,i2,m)+=uDersV(I,ud,i1)*cPoint(uIndex(I)-p1+i1,vIndex(I)-p2+i2,m);
	      ForI(i) uTempV(i,i2,m)+=uDersV(i,ud,i1)*cPoint(uIndex(i)-p1+i1,vIndex(i)-p2+i2,m);
	    }
	  }
	}
      }
      for( int vd=0; vd<=cmd-ud; vd++ )
      {
	if( p2==3 )
	{// common case
	  ForRw(m) xrV(I,m)=vDersV(I,vd,0)*uTempV(I,0,m)+vDersV(I,vd,1)*uTempV(I,1,m)
	                   +vDersV(I,vd,2)*uTempV(I,2,m)+vDersV(I,vd,3)*uTempV(I,3,m);
	}
	else
	{
          //  xrV.redim(I,Rw);  // *wdh*
	  xrV(I,Rw)=0.;
	  for( i2=0; i2<=p2; i2++ )
	    ForRw(m) xrV(I,m)+=vDersV(I,vd,i2)*uTempV(I,i2,m);
	}
	if( ud==0 && vd==0 )
	{
	  xV(I,Rw)=xrV(I,Rw);  // save for derivative with non-uniform weights
	  if( nonUniformWeights )
	    ForR(m) xV(I,m)/=xV(I,nw);
	  if( computeMap )
	    ForR(m) x(I,m)=xV(I,m);
	}
	else
	{
	  if( nonUniformWeights )
	    ForR(m) xr(I,m,vd)=(xrV(I,m) - xV(I,m)*xrV(I,nw))/xV(I,nw);
	  else
	    ForR(m) xr(I,m,vd)=xrV(I,m);
	}
      }
    }
  }
  if( computeMapDerivative )
  {
    for( axis=0; axis<domainDimension; axis++ )
    {
      if( reScale[axis] )
	xr(I,R,axis)*=rScale[axis];
    }
  }
  
  // x.display("nurbs:map: x");
  // xr.display("nurbs:map: xr");

}

