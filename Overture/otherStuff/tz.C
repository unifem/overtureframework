//==========================================================================================
//   Test out the Twilight-Zone Classes
//      OGPolyFunction and OGTrigFunction 
//==========================================================================================
#include "Overture.h"
#include "OGgetIndex.h"
#include "OGTrigFunction.h"
#include "OGPolyFunction.h"
#include "OGPulseFunction.h"
#include "display.h"

// Here is a useful macro for looping over the boundaries
#define ForBoundary(side,axis)   for( axis=0; axis<cg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

#define UX(x,y,z,n) (   \
(   -e(x+2.*h,y,z,n)  \
    + 8.*e(x+   h,y,z,n)  \
    - 8.*e(x-   h,y,z,n)   \
    +e(x-2.*h,y,z,n) )*(1./(12.*h))  )

#define UY(x,y,z,n) (   \
(       -e(x,y+2.*h,z,n)  \
    + 8.*e(x,y+   h,z,n)  \
    - 8.*e(x,y-   h,z,n)   \
        +e(x,y-2.*h,z,n) )*(1./(12.*h))  )

#define UX0(x,y,z,n,t) (   \
(   -e(x+2.*h,y,z,n,t)  \
    + 8.*e(x+   h,y,z,n,t)  \
    - 8.*e(x-   h,y,z,n,t)   \
    +e(x-2.*h,y,z,n,t) )*(1./(12.*h))  )

#define UX3(x,y,z,n) (   \
(   -e(x+2.*h3,y,z,n)  \
    + 8.*e(x+   h3,y,z,n)  \
    - 8.*e(x-   h3,y,z,n)   \
    +e(x-2.*h3,y,z,n) )*(1./(12.*h3))  )


#define UXX(x,y,z,n) (   \
(   -e(x+2.*h,y,z,n)  \
    +16.*e(x+   h,y,z,n)  \
    -30.*e(x     ,y,z,n)  \
    +16.*e(x-   h,y,z,n)   \
    -e(x-2.*h,y,z,n) )*(1./(12.*h*h))  )

#define UXX3(x,y,z,n) (   \
(   -e(x+2.*h3,y,z,n)  \
    +16.*e(x+   h3,y,z,n)  \
    -30.*e(x      ,y,z,n)  \
    +16.*e(x-   h3,y,z,n)   \
    -e(x-2.*h3,y,z,n) )*(1./(12.*h3*h3))  )

#define UXX4(x,y,z,n) (   \
(   -e(x+2.*h4,y,z,n)  \
    +16.*e(x+   h4,y,z,n)  \
    -30.*e(x     ,y,z,n)  \
    +16.*e(x-   h4,y,z,n)   \
    -e(x-2.*h4,y,z,n) )*(1./(12.*h4*h4))  )


OGFunction *ep;


// ============================================================================================
//  Compute errors in TZ array generated values d by comparing to pointwise values 
//
//  Special case : ntd=-1 : check laplacian
// ============================================================================================
real
computeError( MappedGrid & c, const realArray & d, const int & n, const real & t,
              const int & ntd, const int & nxd, const int & nyd, const int & nzd )
{
  
  const realArray & center = c.center();
  Index I1,I2,I3;
  
  assert( ep!=NULL );
  OGFunction & e = *ep;
  
  getIndex( c.indexRange(),I1,I2,I3 );
    
  realArray de(d);
  for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
      {
	if( ntd!=-1 )
	{
	  if( c.numberOfDimensions()==1 )
	    de(i1,i2,i3)=e.gd(ntd,nxd,nyd,nzd,center(i1,i2,i3,0),0.,0.,n,t);
	  else if( c.numberOfDimensions()==2 )
	    de(i1,i2,i3)=e.gd(ntd,nxd,nyd,nzd,center(i1,i2,i3,0),center(i1,i2,i3,1),0.,n,t);
	  else
	    de(i1,i2,i3)=e.gd(ntd,nxd,nyd,nzd,center(i1,i2,i3,0),center(i1,i2,i3,1),center(i1,i2,i3,2),n,t);
	}
	else
	{
           // special case -- check laplacian
	  if( c.numberOfDimensions()==1 )
	    de(i1,i2,i3)=e.gd(0,2,0,0,center(i1,i2,i3,0),0.,0.,n,t);
	  else if( c.numberOfDimensions()==2 )
	    de(i1,i2,i3)=e.gd(0,2,0,0,center(i1,i2,i3,0),center(i1,i2,i3,1),0.,n,t)+
                         e.gd(0,0,2,0,center(i1,i2,i3,0),center(i1,i2,i3,1),0.,n,t);
	  else
	    de(i1,i2,i3)=e.gd(0,2,0,0,center(i1,i2,i3,0),center(i1,i2,i3,1),center(i1,i2,i3,2),n,t)+
                         e.gd(0,0,2,0,center(i1,i2,i3,0),center(i1,i2,i3,1),center(i1,i2,i3,2),n,t)+
                         e.gd(0,0,0,2,center(i1,i2,i3,0),center(i1,i2,i3,1),center(i1,i2,i3,2),n,t);

	}
	
      }

  real error=max(fabs(d-de));

  return error;
}





int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture

  aString nameOfOGFile="square5.hdf";

  int debug=0;

  const int maxNumberOfGridsToTest=2;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "cic", "cicCC" };
  int degreeX=-1;
  if( argc > 1 )
  { 
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      aString input=argv[i];
      if( len=input.matches("degreeX=") )
      {
        sScanF(input(len,input.length()-1),"%i",&degreeX);
	printf(" Degree of space polynomial =%i \n",degreeX);
      }
      else
      {
	numberOfGridsToTest=1;
	gridName[0]=argv[i];
      }
    }
  }
  else
    cout << "Usage: `tz [<gridName>][degreeX=%i]' \n";

  real maxError=0.;

  for( int it=0; it<numberOfGridsToTest; it++ )
  {
    aString nameOfOGFile=gridName[it];

    cout << "\n *****************************************************************\n";
    cout << " ******** Checking grid: " << nameOfOGFile << " ************ \n";
    cout << " *****************************************************************\n\n";


    CompositeGrid cg;
    getFromADataBase(cg,nameOfOGFile);
    cg.update();
    MappedGrid & mg = cg[0];

    const int & numberOfDimensions = cg.numberOfDimensions();
    int nComp = 3;

    RealArray fx(nComp), fy(nComp), fz(nComp), ft(nComp);
    fx=1.;   fx(0)=.5; fx(1)=1.5;
    fy=.1;   fy(0)=.4; fy(1)= .5;
    if( numberOfDimensions==3 )
    {
      fz=1.;   fz(0)=.3; fz(1)=-.3;
    }
    else
    {
      fz=0;
    }
  
    ft=1.;   ft(0)=.6; ft(1)=.35;
  
    OGTrigFunction trigTZ(fx, fy, fz, ft);        //  defines cos(pi*x)*cos(pi*y)*cos(pi*z)*cos(pi*t)

    RealArray gx(nComp), gy(nComp), gz(nComp), gt(nComp);
    gx=1.;   gx(0)=.5; gx(1)=1.5;
    gy=.1;   gy(0)=.4; gy(1)= .5;
    if( numberOfDimensions==3 )
    {
      gz=1.;   gz(0)=.3; gz(1)=-.3;
    }
    else
    {
      gz=0.;
    }
    gt=1.;   gt(0)=.6; gt(1)=.35;
    trigTZ.setShifts(gx,gy,gz,gt);
  
    RealArray amp(nComp);
    amp=1.;
    amp(0)=.25;
    trigTZ.setAmplitudes(amp);

    RealArray cc(nComp);
    cc=1.;
    cc(0)=-.5;
    trigTZ.setConstants(cc);
  
  

    int degreeOfSpacePolynomial = degreeX>=0 ? degreeX : 4;
    int degreeOfTimePolynomial = 3;
    OGPolyFunction polyTZ(degreeOfSpacePolynomial, numberOfDimensions, nComp, degreeOfTimePolynomial);

    RealArray c,a;
    int ndc=degreeOfSpacePolynomial+1;
    c.redim(ndc,ndc,ndc,nComp); c=1.;
    int m1;
    for( m1=0; m1<ndc; m1++ )
    for( int m2=0; m2<ndc; m2++ )
    for( int m3=0; m3<ndc; m3++ )
      for( int n=0; n<nComp; n++ )
      {
	c(m1,m2,m3,n)=1./( m1*m1 + 2.*m2*m2 + 3.*m3*m3 + n+1.);
      }
    

    int ndt=degreeOfTimePolynomial+1;
    a.redim(ndt,nComp); a=1.;
    for( m1=0; m1<ndt; m1++ )
      for( int n=0; n<nComp; n++ )
      {
	a(m1,n)=1./( 2.*m1*m1 + n+1.);
      }

    // c.display("c");
    // a.display("a");

    polyTZ.setCoefficients( c,a );

    real a0=1.2, a1=5., p=2.2;
//    OGPulseFunction pulse(numberOfDimensions, nComp, a0, a1, .1, .2, .3, .4, .5, .6, p);
    OGPulseFunction pulse(numberOfDimensions, nComp );
    p=2.;
    pulse.setShape(p);

    Range all;
    Index I1, I2, I3;
    int grid,n;
    real t=.2;
    
    for( int test=0; test<=2; test++ )
//    for( int test=2; test<=2; test++ )
//    for( int test=0; test<=0; test++ )
    {
      printF("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      if( test==0 )
        printF("++++++++++++++++++ testing OGPolyFunction +++++++++++++++++++\n");
      else if( test==1 )
	printF(" ++++++++++++++++++ testing OGTrigFunction +++++++++++++++++++\n");
      else 
	printF(" ++++++++++++++++++ testing OGPulseFunction +++++++++++++++++++\n");
      printF("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

	
      OGFunction & e = test==0 ? (OGFunction&)polyTZ : test==1 ? (OGFunction&)trigTZ : 
	(OGFunction&)pulse ; // e stands for "exact solution"
      ep = &e;

      // Test the derivatives

      getIndex(mg.dimension(),I1,I2,I3,-1);

      real error;

      real h=pow(REAL_EPSILON/20.,1./5.); // optimal h for 4th order differences
      cout << "h=" << h << endl;

      real h2=pow(REAL_EPSILON/20.,1./6.); // optimal h for 4th order differences for a 2nd derivative
      cout << "h2=" << h2 << endl;
    
      real h3=pow(REAL_EPSILON/20.,1./7.); // optimal h for 4th order differences for a 3rd derivative
      cout << "h3=" << h3 << endl;
    
      real h4=pow(REAL_EPSILON/20.,1./8.); // optimal h for 4th order differences for a 4th derivative
      cout << "h4=" << h4 << endl;

    
      real x=.3, y=.7, z= cg.numberOfDimensions()!=3 ? 0. : .4;
      for( n=0; n<3; n++ )
      {
	printF("--------------------- Test e.AB(x,y,z,n,t) component n=%i ------------------------------------\n",n);

	real t=.1;
	error=fabs(e.t(x,y,z,n,t)-( 
	      -e(x,y,z,n,t+2.*h)
	  + 8.*e(x,y,z,n,t+h   )
	  - 8.*e(x,y,z,n,t-h   ) 
	      +e(x,y,z,n,t-2.*h)
	  )*(1./(12.*h)));
	maxError=max(maxError,error);
	cout << "Error in e.t(" << n << ")= " << error <<endl;


        // *wdh* 090828
	error=fabs(e.gd(2,0,0,0,x,y,z,n,t)-( 
	      -e(x,y,z,n,t+2.*h)
	  +16.*e(x,y,z,n,t+   h)
	  -30.*e(x,y,z,n,t     ) 
	  +16.*e(x,y,z,n,t-   h) 
	      -e(x,y,z,n,t-2.*h)
	  )*(1./(12.*h*h)));

	maxError=max(maxError,error);
	cout << "Error in e.tt(" << n << ")= " << error <<endl;

	if( false )
	{
          real t0=0., x0=0., y0=0., z0=0.;
          x0=REAL_MIN*1000.; y0=2.*x0, z0=0.;

	  printF(" u.t (%8.2e,%8.2e,%8.2e) = %9.3e\n",x0,y0,z0,e.gd(1,0,0,0,x0,y0,z0,n,t0));
	  printF(" u.tt(%8.2e,%8.2e,%8.2e) = %9.3e\n",x0,y0,z0,e.gd(2,0,0,0,x0,y0,z0,n,t0));
	  printF(" u.x (%8.2e,%8.2e,%8.2e) = %9.3e\n",x0,y0,z0,e.gd(0,1,0,0,x0,y0,z0,n,t0));
	  printF(" u.xx(%8.2e,%8.2e,%8.2e) = %9.3e\n",x0,y0,z0,e.gd(0,2,0,0,x0,y0,z0,n,t0));
	  printF(" u.xy(%8.2e,%8.2e,%8.2e) = %9.3e\n",x0,y0,z0,e.gd(0,1,1,0,x0,y0,z0,n,t0));
	  printF(" u.yy(%8.2e,%8.2e,%8.2e) = %9.3e\n",x0,y0,z0,e.gd(0,0,2,0,x0,y0,z0,n,t0));

	  if( true ) return 0;

	}


	error=fabs(e.x(x,y,z,n)-( 
	  -e(x+2.*h,y,z,n)
	  + 8.*e(x+   h,y,z,n)
	  - 8.*e(x-   h,y,z,n) 
	  +e(x-2.*h,y,z,n)
	  )*(1./(12.*h)));
	maxError=max(maxError,error);
	cout << "Error in e.x(" << n << ")= " << error <<endl;

	error=fabs(e.y(x,y,z,n)-( 
	  + 8.*(e(x,y+   h,z,n)
		-e(x,y-   h,z,n))
	  +(e(x,y-2.*h,z,n)
	    -e(x,y+2.*h,z,n))
	  )*(1./(12.*h)));
	maxError=max(maxError,error);
	cout << "Error in e.y(" << n << ")= " << error <<endl;

	if( cg.numberOfDimensions()==3 )
	{
	  error=fabs(e.z(x,y,z,n)-( 
	    + 8.*(e(x,y,z+   h,n)
		  -e(x,y,z-   h,n))
	    +(e(x,y,z-2.*h,n)
	      -e(x,y,z+2.*h,n))
	    )*(1./(12.*h)));
	  maxError=max(maxError,error);
	  cout << "Error in e.z(" << n << ")= " << error <<endl;
	}


	error=fabs(e.xx(x,y,z,n)-( 
	  -e(x+2.*h,y,z,n)
	  +16.*e(x+   h,y,z,n)
	  -30.*e(x     ,y,z,n) 
	  +16.*e(x-   h,y,z,n) 
	  -e(x-2.*h,y,z,n)
	  )*(1./(12.*h*h)));

	maxError=max(maxError,error);
	cout << "Error in e.xx(" << n << ")= " << error <<endl;

	error=fabs(e.yy(x,y,z,n)-( 
	  -e(x,y+2.*h,z,n)
	  +16.*e(x,y+   h,z,n)
	  -30.*e(x,y     ,z,n) 
	  +16.*e(x,y-   h,z,n) 
	  -e(x,y-2.*h,z,n)
	  )*(1./(12.*h*h)));
	maxError=max(maxError,error);
	cout << "Error in e.yy(" << n << ")= " << error <<endl;

	if( cg.numberOfDimensions()==3 )
	{
	  error=fabs(e.zz(x,y,z,n)-( 
	    -e(x,y,z+2.*h,n)
	    +16.*e(x,y,z+   h,n)
	    -30.*e(x,y,z     ,n) 
	    +16.*e(x,y,z-   h,n) 
	    -e(x,y,z-2.*h,n)
	    )*(1./(12.*h*h)));
	  maxError=max(maxError,error);
	  cout << "Error in e.zz(" << n << ")= " << error <<endl;
	}


	error=fabs(e.xy(x,y,z,n)-( 
	  + 8.*(UX(x,y+   h,z,n)
		-UX(x,y-   h,z,n))
	  +(UX(x,y-2.*h,z,n)
	    -UX(x,y+2.*h,z,n))
	  )*(1./(12.*h)));
	maxError=max(maxError,error);
	cout << "Error in e.xy(" << n << ")= " << error <<endl;

	if( cg.numberOfDimensions()==3 )
	{
	  error=fabs(e.xz(x,y,z,n)-( 
	    + 8.*(UX(x,y,z+   h,n)
	         -UX(x,y,z-   h,n))
	        +(UX(x,y,z-2.*h,n)
	         -UX(x,y,z+2.*h,n))
	    )*(1./(12.*h)));
	  maxError=max(maxError,error);
	  cout << "Error in e.xz(" << n << ")= " << error <<endl;

	  error=fabs(e.yz(x,y,z,n)-( 
	    + 8.*(UY(x,y,z+   h,n)
	         -UY(x,y,z-   h,n))
	        +(UY(x,y,z-2.*h,n)
	         -UY(x,y,z+2.*h,n))
	    )*(1./(12.*h)));
	  maxError=max(maxError,error);
	  cout << "Error in e.yz(" << n << ")= " << error <<endl;


	}

	error=fabs(e.xxx(x,y,z,n)-( 
	  -UX3(x+2.*h3,y,z,n)
	  +16.*UX3(x+   h3,y,z,n)
	  -30.*UX3(x      ,y,z,n) 
	  +16.*UX3(x-   h3,y,z,n) 
	  -UX3(x-2.*h3,y,z,n)
	  )*(1./(12.*h3*h3)));

	maxError=max(maxError,error/(fabs(e.xxx(x,y,z,n))+1.));  // relative error
	cout << "Error in e.xxx(" << n << ")= " << error << ", e.xxx=" << e.xxx(x,y,z,n)
	     << ", diff aprox = " << ( 
	       -UX(x+2.*h,y,z,n)
	       +16.*UX(x+   h,y,z,n)
	       -30.*UX(x     ,y,z,n) 
	       +16.*UX(x-   h,y,z,n) 
	       -UX(x-2.*h,y,z,n)
	       )*(1./(12.*h*h)) <<endl;

	error=fabs(e.xxxx(x,y,z,n)-( 
	  -UXX4(x+2.*h4,y,z,n)
	  +16.*UXX4(x+   h4,y,z,n)
	  -30.*UXX4(x      ,y,z,n) 
	  +16.*UXX4(x-   h4,y,z,n) 
	  -UXX4(x-2.*h4,y,z,n)
	  )*(1./(12.*h4*h4)));

	maxError=max(maxError,error/(fabs(e.xxxx(x,y,z,n))+1.)); // relative error
  
	cout << "Error in e.xxxx(" << n << ")= " << error << ", e.xxxx=" << e.xxxx(x,y,z,n) 
	     << ", diff aprox = " <<( 
	       -UXX4(x+2.*h4,y,z,n)
	       +16.*UXX4(x+   h4,y,z,n)
	       -30.*UXX4(x      ,y,z,n) 
	       +16.*UXX4(x-   h4,y,z,n) 
	       -UXX4(x-2.*h4,y,z,n)
	       )*(1./(12.*h4*h4)) 
	     << endl;

	error=fabs(e.gd(0,1,0,0,x,y,z,n)-( 
	  -e(x+2.*h,y,z,n)
	  + 8.*e(x+   h,y,z,n)
	  - 8.*e(x-   h,y,z,n) 
	  +e(x-2.*h,y,z,n)
	  )*(1./(12.*h)));
	maxError=max(maxError,error);
    
	cout << "Error in (.x) e.gd(0,nxd=1,0,0,n=" << n << ")= " << error 
	     << ", e.gd=" << e.gd(0,1,0,0,x,y,z,n) 
	     << ", e.x=" << e.x(x,y,z,n) <<endl;

	error=fabs(e.gd(0,0,1,0,x,y,z,n)-( 
	  + 8.*(e(x,y+   h,z,n)
		-e(x,y-   h,z,n))
	  +(e(x,y-2.*h,z,n)
	    -e(x,y+2.*h,z,n))
	  )*(1./(12.*h)));
	maxError=max(maxError,error);
	cout << "Error in (.y) e.gd(0,0,nyd=1,0,n=" << n << ")= " << error <<endl;

	error=fabs(e.gd(0,0,0,1,x,y,z,n)-( 
	  + 8.*(e(x,y,z+   h,n)
		-e(x,y,z-   h,n))
	  +(e(x,y,z-2.*h,n)
	    -e(x,y,z+2.*h,n))
	  )*(1./(12.*h)));
	maxError=max(maxError,error);
	cout << "Error in (.z) e.gd(0,0,0,nzd=1,n=" << n << ")= " << error 
	     << " e.gd=" << e.gd(0,0,0,1,x,y,z,n) << endl;

	error=fabs(e.gd(1,0,0,0,x,y,z,n)-( 
	  -e(x,y,z,n, 2.*h)
	  + 8.*e(x,y,z,n,    h)
	  - 8.*e(x,y,z,n,   -h) 
	  +e(x,y,z,n,-2.*h)
	  )*(1./(12.*h)));
	maxError=max(maxError,error);
	cout << "Error in (.t) e.gd(ntd=1,0,0,0,n=" << n << ")= " << error <<endl;


	error=fabs(e.gd(0,1,1,0,x,y,z,n)-( 
	  + 8.*(UX(x,y+   h,z,n)
		-UX(x,y-   h,z,n))
	  +(UX(x,y-2.*h,z,n)
	    -UX(x,y+2.*h,z,n))
	  )*(1./(12.*h)));
	maxError=max(maxError,error);
	cout << "Error in (.xy) e.gd(0,nxd=1,nyd=1,0,n=" << n << ")= " << error <<endl;

	if( test!=2 )
	{
	  error=fabs(e.gd(1,1,0,0,x,y,z,n)-
		     ( 
		       -UX0(x,y,z,n, 2.*h)
		       + 8.*UX0(x,y,z,n,    h)
		       - 8.*UX0(x,y,z,n,   -h) 
		       +UX0(x,y,z,n,-2.*h)
		       )*(1./(12.*h)));


	  maxError=max(maxError,error);
	  cout << "Error in (.tx) e.gd(ntd=1,nxd=1,0,0,n=" << n << ")= " << error 
	       << ", e.gd=" << e.gd(1,1,0,0,x,y,z,n) 
	       << ", diff=" <<
	    ( 
	      -UX0(x,y,z,n, 2.*h)
	      + 8.*UX0(x,y,z,n,    h)
	      - 8.*UX0(x,y,z,n,   -h) 
	      +UX0(x,y,z,n,-2.*h)
	      )*(1./(12.*h))
	       << endl;
	  
	}
	
      } 

      printf(" ==========================================\n"
	     "  Test derivatives on a MappedGrid        \n"
	     " ==========================================\n");

      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & c = cg[grid];
    
	getIndex( c.indexRange(),I1,I2,I3 );
    
	for( n=0; n<3; n++ )
	{
	  printF("----------------test e.AA(mg,I1,I2,I3,n,t) component n=%i --------------------------------\n",n);
    
	  error=computeError(c,e(c,I1,I2,I3,n,t),n,t, 0,0,0,0);
	  maxError=max(maxError,error);
	  cout << "Error in e(" << n << ")= " << error <<endl;

	  error=computeError(c,e.t(c,I1,I2,I3,n,t),n,t, 1,0,0,0);
	  maxError=max(maxError,error);
	  cout << "Error in e.t(" << n << ")= " << error <<endl;

	  error=computeError(c,e.x(c,I1,I2,I3,n,t),n,t, 0,1,0,0);
	  maxError=max(maxError,error);
	  cout << "Error in e.x(" << n << ")= " << error <<endl;

	  error=computeError(c,e.y(c,I1,I2,I3,n,t),n,t, 0,0,1,0);
	  maxError=max(maxError,error);
	  cout << "Error in e.y(" << n << ")= " << error <<endl;

	  error=computeError(c,e.laplacian(c,I1,I2,I3,n,t),n,t, -1,0,0,0);
	  maxError=max(maxError,error);
	  cout << "Error in e.laplacian(" << n << ")= " << error <<endl;
	  // OV_ABORT("finish me...");


	  error=computeError(c,e.gd(0,1,0,0, c,I1,I2,I3,n,t),n,t, 0,1,0,0);
	  maxError=max(maxError,error);
	  cout << "Error in e.gd(0,1,0,0,n=" << n << ")= " << error <<endl;

	  error=computeError(c,e.gd(0,0,1,0, c,I1,I2,I3,n,t),n,t, 0,0,1,0);
	  maxError=max(maxError,error);
	  cout << "Error in e.gd(0,0,1,0,n=" << n << ")= " << error <<endl;


          if( test!=2 )
	  {
	    error=computeError(c,e.gd(0,1,1,0, c,I1,I2,I3,n,t),n,t, 0,1,1,0);
	    maxError=max(maxError,error);
	    cout << "Error in e.gd(0,1,1,0,n=" << n << ")= " << error <<endl;


	    error=computeError(c,e.gd(1,1,1,0, c,I1,I2,I3,n,t),n,t, 1,1,1,0);
	    maxError=max(maxError,error);
	    cout << "Error in e.gd(1,1,1,0,n=" << n << ")= " << error <<endl;
	  }
	  
	} // end for n 
 
	if( test==0 || test==1 ) // Pulse function has limited derivatives 
	{
          // --- Test newer optimized calls ---
	  OV_GET_SERIAL_ARRAY(real,c.center(),xLocal);
	  const bool isRectangular = false; // ** do this for now ** mg.isRectangular();
          Range C=3;
	  RealArray ux(I1,I2,I3,C);
	  
	  const int numDeriv=4;  // check this many derivatives 
	  const int numDerivZ = numberOfDimensions==3 ? numDeriv : 0;
	  // int ntd=0,nxd=0,nyd=3,nzd=0;
	  for( int nzd=0; nzd<=numDerivZ; nzd++ )
	  for( int nyd=0; nyd<=numDeriv; nyd++ )
	  for( int nxd=0; nxd<=numDeriv; nxd++ )
	  for( int ntd=0; ntd<=numDeriv; ntd++ )
	  {
	    
	    e.gd( ux ,xLocal,numberOfDimensions,isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,C,t);
	    for( n=0; n<3; n++ )
	    {
	      error=computeError(c,ux(I1,I2,I3,n),n,t, ntd,nxd,nyd,nzd);
	      maxError=max(maxError,error);
	      printF("__ Error in e.gd( ux,xLocal,..., ntd=%i,nxd=%i,nyd=%i,nzd=%i...) ,n=%d = %9.2e\n",
		     ntd,nxd,nyd,nzd,n,error);
	    }
	  
	    const real errTol=1.e-3;
	    if( fabs(error)>errTol ) 
	    {
	      OV_ABORT("TROUBLE -- large error!");
	    }
	  }
	  
	}
	

      }

    }  // end poly/trig loop
    

    if( FALSE )
      return 0;

  //    ==========================================
  //     Test assigning a MappedGridFunction
  //    ==========================================
    realMappedGridFunction u(mg,all,all,all,3);  	// Create a mappedGridFunction and set its values
    u=0.;
    getIndex(mg.dimension(),I1,I2,I3);

    n = 0;
    t = 0.;
    Range ND;
    ND = Range(0,mg.numberOfDimensions()-1);

    u(I1,I2,I3,ND) = trigTZ(mg,I1,I2,I3,ND,t);   // Assign some components of u
    if( debug & 2 )
      display(u,"OGTrigFunction solution from trigTZ(mg,I1,I2,I3,ND,t)","%4.1f ");

    u(I1,I2,I3,ND) = polyTZ(mg,I1,I2,I3,ND,t);
    if( debug & 2 )
      display(u,"OGPolyFunction solution from polyTZ(mg,I1,I2,I3,ND,t)","%4.1f ");

    u=-1.;
    u(I1,I2,I3,0) = polyTZ(mg,I1,I2,I3,0,t);
    if( debug & 2 )
      display(u,"OGPolyFunction solution from polyTZ(mg,I1,I2,I3,0,t)","%4.1f ");


    //  ===== evaluate the function on the faces along axis1
    getIndex(mg.dimension(),I1,I2,I3,-1);  // can't do all points for face centered
    u(I1,I2,I3,ND) = trigTZ(mg,I1,I2,I3,ND,t,GridFunctionParameters::faceCenteredAxis1); 
    if( debug & 2 )
      display(u,"OGTrigFunction solution from trigTZ(mg,I1,I2,I3,ND,t,GridFunctionParameters::faceCenteredAxis1)","%4.1f ");

    //    ===========================================
    //     Now test assigning a CompositeGridFunction
    //    ===========================================
    realCompositeGridFunction v;    // Create a CompositeGridFunction and set values
    v=0.;
    if( FALSE )
    {
      for( int loop=0; loop<300; loop++ )
      {
	v=trigTZ(cg,0,t);               // v ...
	if( loop % 30 == 0 )
	  printf("Number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
      }
    }
    if( debug & 2 )
      v.display("OGTrigFunction solution after trigTZ(cg,0,t);","%4.1f ");

    v=0.;
    v=polyTZ(cg,0,t);
    real error=0.;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].dimension(),I1,I2,I3);
      error=max(error,max(abs(v[grid](I1,I2,I3)-polyTZ(cg[grid],I1,I2,I3,0,t))));
    }
    
    cout << "Error in CompositeGridFunction=TZ(cg) = " << error <<endl;
    maxError=max(maxError,error);
    if( debug & 2 )
      v.display("OGPolyFunction solution after polyTZ(cg,0,t)","%4.1f ");

    v=0.;
    trigTZ.assignGridFunction(v,t);
    error=0.;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].dimension(),I1,I2,I3);
      error=max(error,max(abs(v[grid](I1,I2,I3)-trigTZ(cg[grid],I1,I2,I3,0,t))));
    }
    cout << "Error in assignGridFunction(v) = " << error <<endl;
    if( debug & 2 )
    {
      v.display("OGPolyFunction solution after assignGridFunction(v)","%4.1f ");
      polyTZ(cg,0,t).display("Here is polyTZ(cg,0,t)");
    }
    
    
    Index N(1,2);
    realCompositeGridFunction w(cg,all,all,all,N);    // Create a CompositeGridFunction and set values
    w=polyTZ(cg,N,t);
    if( debug & 2 )
      w.display("OGPolyFunction solution after w=polyTZ(cg,N,t);","%4.1f ");
    error=0.;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].dimension(),I1,I2,I3);
      error=max(error,max(abs(w[grid](I1,I2,I3,N)-polyTZ(cg[grid],I1,I2,I3,N,t))));
    }
    cout << "Error in w=polyTZ(cg,N,t) = " << error <<endl;
  

  //   ======================================================================
  //   In this next section we assign the forcing for the following equations
  //       u.t + u*u.x + v*u.y = f_0
  //       v.t + u*v.x + v*v.y = f_1
  //   with boundary conditions
  //       (u,v) = given for boundaryCondition==1
  //       (u.x,v.x) = given for all other boundary conditions
  //   ======================================================================

    realCompositeGridFunction forcing(cg,all,all,all,2);
    forcing=0.;
    for(grid=0; grid<cg.numberOfComponentGrids(); grid++)
    {
      getIndex(cg[grid].dimension(),I1,I2,I3);                              // assign all points on the grid
      forcing[grid](I1,I2,I3,0)=trigTZ.t(cg[grid],I1,I2,I3,0)
	+trigTZ(cg[grid],I1,I2,I3,0)*trigTZ.x(cg[grid],I1,I2,I3,0)
	+trigTZ(cg[grid],I1,I2,I3,1)*trigTZ.y(cg[grid],I1,I2,I3,0);
      forcing[grid](I1,I2,I3,1)=trigTZ.t(cg[grid],I1,I2,I3,1)
	+trigTZ(cg[grid],I1,I2,I3,0)*trigTZ.x(cg[grid],I1,I2,I3,1)
	+trigTZ(cg[grid],I1,I2,I3,1)*trigTZ.y(cg[grid],I1,I2,I3,1);
      int side,axis;
      ForBoundary(side,axis)                                             // loop over all boundaries 
      {
	getBoundaryIndex(cg[grid].gridIndexRange(),side,axis,I1,I2,I3);
	if( cg[grid].boundaryCondition()(side,axis)==1 )
	{
	  forcing[grid](I1,I2,I3,0)=trigTZ(cg[grid],I1,I2,I3,0);         // Dirichlet
	  forcing[grid](I1,I2,I3,1)=trigTZ(cg[grid],I1,I2,I3,1);
	}	
	else if( cg[grid].boundaryCondition()(side,axis)>0 )
	{
	  forcing[grid](I1,I2,I3,0)=trigTZ.x(cg[grid],I1,I2,I3,0);
	  forcing[grid](I1,I2,I3,1)=trigTZ.x(cg[grid],I1,I2,I3,1);
	}
      }
    }
    if( debug & 2 )
      forcing.display("Here is the forcing function","%4.1f ");

  } // end loop over grids

  printf(" ****************************************************************\n"
	 " **************** Maximum ERROR = %e *******************\n" 
	 " ***************************************************************\n",maxError);
  if( maxError > 2.e-3 )
  {
    printf("\n\n"
	   " ************ The ERROR is large somewhere. Test failed. *************** \n"
	   " *********************************************************************** \n");
  }
  else
  {
    printf("\n\n"
	   " *********************************************************************** \n"
	   " ************ The test is apparently successful ************************ \n"
	   " *********************************************************************** \n");
  }

  Overture::finish();          
  return 0;
}
