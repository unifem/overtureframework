// ====================================================================================
///  \file trb.C
///  \brief test program to the Newton-Euler rigid body evolution equations (RigidBodyMotion.C)
// ===================================================================================


#include "Overture.h"
#include "SquareMapping.h"
#include "PlotStuff.h"
#include "MatrixTransform.h"
#include "CrossSectionMapping.h"
#include "RigidBodyMotion.h"

#include "App.h"

RealArray 
mult( const RealArray & a, const RealArray & b );
RealArray 
trans( const RealArray &a );

// #include <cmath>

enum TestProblemEnum
{
  generalMotion=0,
  trigonometricMotion,  // trig TZ 
  freeRotation1,
  freeRotation2,
  freeRotation3,
  constantX0Acceleration,
  constantX1Acceleration,
  constantX2Acceleration,
  constantW0Acceleration,
  constantW1Acceleration,
  constantW2Acceleration,
  sinX0Acceleration,
  sinX1Acceleration,
  sinX2Acceleration,
  exponentialV,
  fallingSphere
} testProblem=generalMotion;


class TestRigidBody
{

public:

TestRigidBody();
~TestRigidBody();

int 
getErrors( int & numErr, aString *& errorNames, RealArray & err );

int
getExactForcing( const real t, RealArray & f, RealArray & g, 
		 RealArray & A11, RealArray & A12, RealArray & A21, RealArray & A22 );

int 
getExactSolution( const real t, RealArray & xe, RealArray & ve, RealArray & we );

void
getForce( real t, RealArray & f , RealArray & g, 
          RealArray & A11, RealArray & A12, RealArray & A21, RealArray & A22,
          RealArray & xCM, RealArray & vCM );

int 
initialConditions();

int
solve(GenericGraphicsInterface & gi);

int 
output( TestProblemEnum testProblem, const int step );

RigidBodyMotion::TimeSteppingMethodEnum method;

real cfl, tFinal, tPlot, t, dt, dt0;
int orderOfAccuracy;

real mass;           // total mass
RealArray mI;        // 3 moment of inertia

int numberOfDimensions;

RealArray xCM0, vCM0, f0, g0, w0, e0;

int debug;

int plotOption;
int plotBody;
bool saveMatlabFile;  // save results to a matlab file.
bool addedMass;

// parameters for exact solutions
int component;

real cf1, cf2, cf3;
real cfq1, cfq2, cfq3;
	  
// For the free rotation solutions:
int freeRotationAxis;  // 0, 1 or 2
int m1, m2, m3;  // defines a permutation of the exact solution 

real lambda;  // for exponentialV solution

// Falling sphere parameters
real gravity, dragCoeff;

RigidBodyMotion body;

aString testName;

};
  




// real cf1=2., cf2=1.5, cf3=-1.;
// real cfq1=1., cfq2=2., cfq3=.5;
	  
// for the Smart exact solution
// const int m0=0, m1=1, m2=2;  // defines a permutation of the exact solution
//int m0=1, m1=2, m2=0;  // defines a permutation of the exact solution 
// static real lambda=.25;
//static real alpha=.5;
//static real a=1., b=-1., c=.5;

// static int component=0;

TestRigidBody::
TestRigidBody()
  : body(3)
{
  method=RigidBodyMotion::leapFrogTrapezoidal;


  t=0.;
  dt0=.1;
  dt=.1;  // dt = cfl*dt0
  cfl=1.; tFinal=1.; tPlot=.1;
  orderOfAccuracy=2;
  
  debug=1;

  plotOption=true;
  plotBody=true;
  
  saveMatlabFile=true;  // save results to a matlab file.
  addedMass=0;
  
  numberOfDimensions=3;

  mass=1.;           // total mass
  mI.redim(3);        // 3 moment of inertia
  mI(0)=1.; mI(1)=.5; mI(2)=2.;

  xCM0.redim(3); vCM0.redim(3); f0.redim(3); g0.redim(3); w0.redim(3);

  e0.redim(3,3);
  e0=0.;

  // parameters for exact solutions

  component=0;

  cf1=2., cf2=1.5, cf3=-1.;
  cfq1=1., cfq2=2., cfq3=.5;

  
  freeRotationAxis=0;   // default free rotation solution axis
  
  lambda=1.;  // for exponentialV solution 

  // for the Smart exact solution
  // const int m0=0, m1=1, m2=2;  // defines a permutation of the exact solution
  // alpha=.5;
  //  a=1., b=-1., c=.5;

  // Falling sphere parameters
  gravity=-1;   // gravity in -y direction
  dragCoeff=.25;

}

TestRigidBody::
~TestRigidBody()
{
}


void TestRigidBody::
getForce( real t, RealArray & f , RealArray & g, 
          RealArray & A11, RealArray & A12, RealArray & A21, RealArray & A22,
          RealArray & xCM, RealArray & vCM )
{

  A11=0.;  A12=0.; A21=0.; A22=0.;

  if( testProblem==trigonometricMotion )
  {
    // twilightzone forcing is set in the RigidBody class
    f=0.;
    g=0.;

    // Added mass matrices are set here

    if( addedMass )
    {
      real z = 2.; // 2. + .25*sin(2.*Pi*t) ; // "impedance" for added mass 
      // A11 = z*[ 1. 0. 0.; 0. 2. 0.; 0. 0. 3.];
      // A12 = z*[ 0. .1 0.; .2 0. 0.; 0. 0. .3];
      // A21 = A12'; 
      // A22 = z*[ 3. 0. 0.; 0. 2. 0.; 0. 0. 1.];

      A11(0,0)=z*1.; A11(0,1)=z*0.; A11(0,2)=z*0.;
      A11(1,0)=z*0.; A11(1,1)=z*2.; A11(1,2)=z*0.;
      A11(2,0)=z*0.; A11(2,1)=z*0.; A11(2,2)=z*3.;
  
      A12(0,0)=z*0.; A12(0,1)=z*.1; A12(0,2)=z*0.;
      A12(1,0)=z*.2; A12(1,1)=z*0.; A12(1,2)=z*0.;
      A12(2,0)=z*0.; A12(2,1)=z*0.; A12(2,2)=z*.3;

      for( int i=0; i<3; i++ )for( int j=0; j<3; j++ )
      {
	A21(i,j)=A12(j,i);
      }
  
      A22(0,0)=z*3.; A22(0,1)=z*0.; A22(0,2)=z*0.;
      A22(1,0)=z*0.; A22(1,1)=z*2.; A22(1,2)=z*0.;
      A22(2,0)=z*0.; A22(2,1)=z*0.; A22(2,2)=z*1.;
    }
    
  }
  else if( testProblem==constantX0Acceleration|| 
	   testProblem==constantX1Acceleration || 
	   testProblem==constantX2Acceleration )
  {
    f=0.; 
    f(component)=1.;
    g=0.;
  }
  else if( testProblem==constantW0Acceleration ||
	   testProblem==constantW1Acceleration ||
	   testProblem==constantW2Acceleration )
  {
    f=0.;
    g=0.; 
    g(component)=1.;
      
  }
  else if( testProblem==sinX0Acceleration || 
	   testProblem==sinX1Acceleration ||
	   testProblem==sinX2Acceleration )
  {
    f=0.;
    f(component)=-sin(t);
    g=0.;
  }
  else if( testProblem==generalMotion )
  {
    // 
    // 
//     f(0)=sin(Pi*t);
//     f(1)=.5*sin(  Pi*t);
//     f(2)=.5*sin(.5*Pi*t);

//     g(0)=cos(Pi*t);
//     g(1)=cos(.5*Pi*t);
//     g(2)=cos(2.*Pi*t);

    f(0)=cf1*sin(cfq1*Pi*t);
    f(1)=cf2*sin(cfq2*Pi*t);
    f(2)=cf3*sin(cfq3*Pi*t);

    g(0)=     cos(   Pi*t);
    g(1)=1.25*cos(.5*Pi*t);
    g(2)=-.75*cos(2.*Pi*t);
  }
  else if( testProblem==exponentialV )
  {
    f(0)= lambda*vCM(0); 
    f(1)= lambda*vCM(1);
    f(2)= lambda*vCM(2);

    g(0)=0.;
    g(1)=0.;
    g(2)=0.;


  }
  else if( testProblem==freeRotation1 ||
           testProblem==freeRotation2 ||
           testProblem==freeRotation3  )
  {
    f(0)=0.;
    f(1)=0.;
    f(2)=0.;

    g(0)=0.;
    g(1)=0.;
    g(2)=0.;

  }
  else if( testProblem==fallingSphere )
  {
    // Sphere falling with drag
    // Force = m*g - Cd*.5*rho*v^2   
    //    m=1, Cd=.5
    f(0)=0.;
    f(1)=gravity*mass + dragCoeff*SQR(vCM(1));
    f(2)=0.;

    g(0)=0.;
    g(1)=0.;
    g(2)=0.;

    if( addedMass )
    {
      // added mass matrix for the falling sphere -- is this right?

      A11(1,1) = -dragCoeff*vCM(1);
      f(1) += A11(1,1)*vCM(1);
    }
	


  }
  

}

// ==================================================================================
//  Setup and assign initial conditions.
// ==================================================================================
int TestRigidBody::
initialConditions()
{

  e0(0,0)=1.; e0(1,0)=0.; e0(2,0)=0.;
  e0(0,1)=0.; e0(1,1)=1.; e0(2,1)=0.;
  e0(0,2)=0.; e0(1,2)=0.; e0(2,2)=1.;

  xCM0=0.;
  vCM0=0.;
  w0=0.;

  body.setTwilightZone( false );  // By default we do not use TZ

  if( testProblem==trigonometricMotion )
  {
    testName="TrigTZ";
    
    // Turn on trig twilightzone
    body.setTwilightZone( true, RigidBodyMotion::trigonometricTwilightZone );


    // Set initial conditions
    body.getExactSolution( 0,t,xCM0,vCM0,w0 );

    printF(" Initial conditions: x=(%8.2e,%8.2e,%8.2e) v=(%8.2e,%8.2e,%8.2e)\n",xCM0(0),xCM0(1),xCM0(2),
	   vCM0(0),vCM0(1),vCM0(2));


  }
  else if( testProblem==sinX0Acceleration ||
	   testProblem==sinX1Acceleration ||
	   testProblem==sinX2Acceleration )
  {
    if( testProblem==sinX0Acceleration )
      testName="SinX0Accel";
    else if( testProblem==sinX1Acceleration )
      testName="SinX1Accel";
    else
      testName="SinX2Accel";


    component=(int)testProblem-(int)sinX0Acceleration;
    // dt=cfl*.01;
    vCM0(component)=+1.;
  }
  else if( testProblem==constantX0Acceleration || 
	   testProblem==constantX1Acceleration || 
	   testProblem==constantX2Acceleration )
  {
    if( testProblem==constantX0Acceleration )
      testName="ConstX0Accel";
    else if( testProblem==constantX1Acceleration )
      testName="ConstX1Accel";
    else
      testName="ConstX2Accel";

    component=(int)testProblem-(int)constantX0Acceleration;
    // dt=cfl*.01;
  }
  else if( testProblem==constantW0Acceleration ||
	   testProblem==constantW1Acceleration ||
	   testProblem==constantW2Acceleration )
  {
    if( testProblem==constantW0Acceleration )
      testName="ConstW0Accel";
    else if( testProblem==constantW1Acceleration )
      testName="ConstW1Accel";
    else
      testName="ConstW2Accel";

    component=(int)testProblem-(int)constantW0Acceleration;
  }
  else if( testProblem==exponentialV )
  {
    // solve :  m v' = lambda *v 
    //          x' = v 
      
    testName="exponentialV";

    // dt=.1*cfl/fabs(lambda);
      
    vCM0(0)=1.;
    vCM0(1)=2.;
    vCM0(2)=3.;
      
    xCM0(0)=3.;
    xCM0(1)=2.;
    xCM0(2)=1.;
      

  }
  else if( testProblem==generalMotion )
  {
    testName="generalMotion";

    // numberOfSteps=2000;
//       vCM(0)=-1./(Pi*mass);
//       vCM(1)=-.5/(Pi*mass);
//       vCM(2)=-1./(Pi*mass);
  }
//       else if( answer=="cfl" )
//       {
// 	ps.inputString(answer,sPrintF("Enter the cfl number (current=%6.4f)",cfl));
// 	sScanF(answer,"%e",&cfl);
// 	printf(" cfl=%6.2f \n",cfl);
// 	continue;
//       }
  // else if( testProblem==smartExactSolution )
  else if( testProblem==freeRotation1 ||
	   testProblem==freeRotation2 ||
	   testProblem==freeRotation3 )
  {
    // Free rotation solution (Exact solution from ref E.H. Smart, Advanced Dynamics Vol II)
    // 
    // Here is is a generalization of the "Smart" solution:   
    // NOTE: This is the solution for wHat(t) in the rotating frame, wHat = E^T w 
    //    wHat1 = A cos(alpha*t) - C sigma*sin(alpha*t)
    //    wHat2 = C cos(alpha*t) + A sigma sin(alpha*t)
    //    wHat3 = wHat3(0)
    //
    //    A = wHat1(0), C=wHat2(0)
    //    alpha = abs( wHat3(0)*(I3-I1)/I1 )
    //    sigma = sgn(wHat3(0)*(I3-I1)/I1)
    // 
    if( testProblem==freeRotation1 )
      testName="freeRotation1";
    else if( testProblem==freeRotation2 )
      testName="freeRotation2";
    else
      testName="freeRotation3";

    vCM0(0)=0.;
    vCM0(1)=0.;
    vCM0(2)=0.;
      
    xCM0(0)=0.;
    xCM0(1)=0.;
    xCM0(2)=0.;
      
//     w0(m0)=a*cos(lambda*t);
//     w0(m1)=b*sin(lambda*t);
//     w0(m2)=c; 
      
//     mI(m0)=2.; mI(m1)=2.; 
//     mI(m2)=mI(m0)-alpha*mI(m0); 

    //  free rotation test 
    // If I1=I2 or I2=I3 or I3=I1 we can compute the exact solution for omegaHat = E^T omegav
    mI=1.;
    mI(freeRotationAxis)=2.;

    m3 = freeRotationAxis;
    m1=  (m3+1) % 3;
    m2=  (m1+1) % 3;

    w0=0.;
    w0(m1)=1.; w0(m2)=2.; w0(m3)=3.; // initial conditions

  }
  else if( testProblem==fallingSphere )
  {
    // Sphere falling to a terminal velocity with a drag force
    testName="fallingSphere";

    xCM0=0.;
    vCM0=0.;
  }
  else
  {
    printF("Unknown testProblem=%i\n",(int)testProblem);
    OV_ABORT("error");
  }

  dt=cfl*dt0;

  // Save the initial conditions and forces

  RealArray A11(3,3), A12(3,3), A21(3,3), A22(3,3);
  A11=0.;  A12=0.; A21=0.; A22=0.;

  assert( t==0. );
  // save the initial force and torque
  getForce( t,f0,g0, A11,A12,A21,A22, xCM0,vCM0 );
    
  body.setProperties(mass,mI,numberOfDimensions);
  body.setInitialConditions(t,xCM0,vCM0,w0,e0);

  return 0;

}


// =========================================================================================
// Output errors and other info.
// =========================================================================================
int TestRigidBody::
output( TestProblemEnum testProblem, const int step )
{
  if( debug==0 ) return 0;

  RealArray xCM(3), vCM(3), rotation(3,3), w(3);
  
  body.getCoordinates( t, xCM, vCM,rotation,w );

  RealArray xe(3),ve(3),we(3);
  getExactSolution( t, xe, ve, we );

  real t0=0.;
  
//   body.getVelocity( t,vCM );
//   body.getAngularVelocities( t,w );
  
 if( testProblem==trigonometricMotion ||
           testProblem==constantX0Acceleration || 
	   testProblem==constantX1Acceleration || 
	   testProblem==constantX2Acceleration ||
	   testProblem==constantW0Acceleration ||
	   testProblem==constantW1Acceleration ||
	   testProblem==constantW2Acceleration ||
	   testProblem==sinX0Acceleration || 
	   testProblem==sinX1Acceleration ||
	   testProblem==sinX2Acceleration ||
	   testProblem==exponentialV      ||
           testProblem==generalMotion  ||
           testProblem==fallingSphere )
  {
    realArray err(3);
    err(0) = max(fabs(xCM-xe));
    err(1) = max(fabs(vCM-ve));
    err(2) = max(fabs(w  -we));
    
    printF("%s: t=%8.2e x=(%9.2e,%9.2e,%9.2e), v=(%9.2e,%9.2e,%9.2e), w=(%9.2e,%9.2e,%9.2e), "
	   "xErr=%8.2e, vErr=%8.2e, wErr=%8.2e\n",
	   (const char*)testName, t,xCM(0),xCM(1),xCM(2),vCM(0),vCM(1),vCM(2),w(0),w(1),w(2),err(0),err(1),err(2));

    if( false && testProblem==exponentialV )
    {
      printF(" v-err=(%8.2e,%8.2e,%8.2e) \n",vCM(0)-ve(0),vCM(1)-ve(1),vCM(2)-ve(2));
      
    }

  }
  else if( testProblem==freeRotation1 ||
	   testProblem==freeRotation2 ||
	   testProblem==freeRotation3 )
  {
    // NOTE: This is the solution for wHat(t) in the rotating frame, wHat = E^T w 
    //    wHat1 = A cos(alpha*t) - C sigma*sin(alpha*t)
    //    wHat2 = C cos(alpha*t) + A sigma sin(alpha*t)
    //    wHat3 = wHat3(0)
    //
    //    A = wHat1(0), C=wHat2(0)
    //    alpha = abs( wHat3(0)*(I3-I1)/I1 )
    //    sigma = sgn(wHat3(0)*(I3-I1)/I1)

    RealArray wHat(3), err(3);
    
    RealArray e(3,3);
    body.getAxesOfInertia( t,e );
    for( int j=0; j<3; j++ )
      wHat(j) = e(0,j)*w(0)+e(1,j)*w(1)+e(2,j)*w(2);
    
    err = fabs(wHat-we);

    printF("FreeRotation%i: t=%8.2e steps=%i w=(%8.3e,%8.3e,%8.3e), weHat=(%8.3e,%8.3e,%8.3e), "
           "wHat=(%8.3e,%8.3e,%8.3e),  wHat-err=(%7.3e,%7.3e,%7.3e) \n",
           freeRotationAxis,t,step,w(0),w(1),w(2),we(0),we(1),we(2),wHat(0),wHat(1),wHat(2),
	   err(0),err(1),err(2));

    // printf("FreeRotation%i: w0=(%8.3e,%8.3e,%8.3e)\n",w0(0),w0(1),w0(2));


  }
  else
  {
    printF("TestRigidBody::output:ERROR: unknown testProblem =%i\n",(int)testProblem);
    OV_ABORT("error");
    
  }

}


int
convergenceRate(const RealArray & h, 
                const RealArray & e,
                RealArray & sigma)
// =========================================================================
//  Make a least squares fit to the convergence rate.
//   h(i), e(i,j)  sigma(j)
// =========================================================================
{
  const int n = h.getLength(0);
  const int m = e.getLength(1);
  
  Range N(0,n-1), M(0,m-1);

  RealArray hh(N), ee(N,M);
  
  hh(N)=log(h);
  ee(N,M)=log(e);
  
  // .............least squares fit to exponent of convergence

  for( int j=0; j<m; j++ )
  {
    real sumH=sum(hh);
    real sumE=sum(ee(N,j));
    real sumHE=sum(hh*ee(N,j));
    real sumH2=sum(hh*hh);
    
    sigma(j) = (sumHE*n-sumH*sumE)/(n*sumH2-sumH*sumH);
    real e0 = (sumH2*sumE-sumH*sumHE)/(n*sumH2-sumH*sumH);
  }

  return 0;
}

// ============================================================================================
/// \brief Compute the exact solution.
// ============================================================================================
int TestRigidBody::
getExactSolution( const real t, RealArray & xe, RealArray & ve, RealArray & we )
{
  xe=0.; ve=0.; we=0.;

  const real t0=0.;

  if( testProblem==trigonometricMotion )
  {
    body.getExactSolution( 0,t,xe,ve,we );
  }
  else if( testProblem==constantX0Acceleration || 
	   testProblem==constantX1Acceleration || 
	   testProblem==constantX2Acceleration )
  {
    xe(component)= .5*f0(component)/mass*SQR(t);
    ve(component)= (f0(component)/mass*t+vCM0(component));

  }
  else if( testProblem==constantW0Acceleration ||
	   testProblem==constantW1Acceleration ||
	   testProblem==constantW2Acceleration )
  {
    we(component)= g0(component)/mI(component)*t;
  }
  else if(  testProblem==sinX0Acceleration || 
	    testProblem==sinX1Acceleration ||
	    testProblem==sinX2Acceleration )
  {
    xe(component) = sin(t)/mass + (vCM0(component) - (cos(t0)/mass))*t + xCM0(component);
    ve(component) = cos(t)/mass+ (vCM0(component) - (cos(t0)/mass));
  }
  else if( testProblem==exponentialV )
  {
    //  v' = lambda*v/m  -->   v = v(0)*exp(lambda/m*t )
    //  x' = v                 x = v(0)*( (m/lambda)*(exp(lambda/m*t)-1.) ) + x(0)
    for( int n=0; n<3; n++ )
    {
      xe(n) = vCM0(n)*( (mass/lambda)*(exp(lambda/mass*t)-1.) ) + xCM0(n);
      ve(n) = vCM0(n)*exp(lambda/mass*t);
    }

  }
  else if( testProblem==generalMotion )
  {
// 	  printf("t=%8.2e x=(%7.2e,%7.2e,%7.2e) error(x)=(%6.1e,%6.1e,%6.1e) error(v)=(%6.1e,%6.1e,%6.1e) \n",t,
// 		 xCM(0),xCM(1),xCM(2), 
// 		 fabs(xCM(0)-( -sin(Pi*t)/(Pi*Pi*mass) )),
// 		 fabs(xCM(1)-( -.5*sin(Pi*t)/(Pi*Pi*mass) )),
// 		 fabs(xCM(2)-( -.5*sin(.5*Pi*t)/(.5*Pi*.5*Pi*mass) )),
// 		 fabs(vCM(0)-(    (1.-cos(   Pi*t))/(Pi*mass)   +vCM0(0))),
// 		 fabs(vCM(1)-( .5*(1.-cos(   Pi*t))/(Pi*mass)   +vCM0(1))),
// 		 fabs(vCM(2)-( .5*(1.-cos(.5*Pi*t))/(.5*Pi*mass)+vCM0(2)))
// 	    );

    // f = cf1*sin(cfq1*t)
    // here is the true solution
    xe(0) = -cf1*sin(cfq1*Pi*t)/(SQR(cfq1*Pi)*mass)  
      + (vCM0(0) + cf1*cos(cfq1*Pi*t0)/(cfq1*Pi*mass) )*t + xCM0(0);
    xe(1) = -cf2*sin(cfq2*Pi*t)/(SQR(cfq2*Pi)*mass)  
      + (vCM0(1) + cf2*cos(cfq2*Pi*t0)/(cfq2*Pi*mass) )*t + xCM0(1);
    xe(2) = -cf3*sin(cfq3*Pi*t)/(SQR(cfq3*Pi)*mass) 
      + (vCM0(2) + cf3*cos(cfq3*Pi*t0)/(cfq3*Pi*mass) )*t + xCM0(2);

    ve(0) =  cf1*(1.-cos(cfq1*Pi*t))/(cfq1*Pi*mass)   +vCM0(0);
    ve(1) =  cf2*(1.-cos(cfq2*Pi*t))/(cfq2*Pi*mass)   +vCM0(1); 
    ve(2) =  cf3*(1.-cos(cfq3*Pi*t))/(cfq3*Pi*mass)   +vCM0(2);

  }
  // else if( testProblem==smartExactSolution )
  else if( testProblem==freeRotation1 ||
	   testProblem==freeRotation2 ||
	   testProblem==freeRotation3 )
  {
    // NOTE: This is the solution for wHat(t) in the rotating frame, wHat = E^T w 
    //    wHat1 = A cos(alpha*t) - C sigma*sin(alpha*t)
    //    wHat2 = C cos(alpha*t) + A sigma sin(alpha*t)
    //    wHat3 = wHat3(0)
    //
    //    A = wHat1(0), C=wHat2(0)
    //    alpha = abs( wHat3(0)*(I3-I1)/I1 )
    //    sigma = sgn(wHat3(0)*(I3-I1)/I1)

    // Exact solution:
    real freq = abs(w0(m3)*(mI(m3)-mI(m1))/mI(m1));
    real scale =   (w0(m3)*(mI(m3)-mI(m1))/mI(m1))/freq;   // sign( w0(3)*(I3-I1)/I1 )

    we(m1) = w0(m1)*cos(freq*t) -scale*w0(m2)*sin(freq*t);
    we(m2) = w0(m2)*cos(freq*t) +scale*w0(m1)*sin(freq*t);
    we(m3) = w0(m3);

  }
  else if( testProblem==fallingSphere )
  {
    // Sphere falling with drag

    const real beta = sqrt(dragCoeff*fabs(gravity)/mass);
    const real alpha = sqrt(fabs(gravity)*mass/dragCoeff);
    
    real betat=beta*t;
    ve(1) = -alpha*tanh(betat); 
    if( betat<100. ) // exp(-100) = 3.7e-44
    {
      xe(1) = -(alpha/beta)*log(cosh(betat));   // trouble evaluating for beta*t >> 1 9 e.g. for small mass
    }
    else
    { // log(cosh(x)) = log( (e^x + e^{-1})/2 ) approx log(e^x/2) = x - log(2).
      xe(1) = -(alpha/beta)*( betat - log(2.) );
    }
  }
  else
  {
    OV_ABORT("finish me");
  }
  
}


// ============================================================================================
/// \brief Compute the exact solution.
// ============================================================================================
int TestRigidBody::
getExactForcing( const real t, RealArray & f, RealArray & g, 
		 RealArray & A11, RealArray & A12, RealArray & A21, RealArray & A22 )
{
  RealArray xe(3),ve(3),we(3);
  getExactSolution( t, xe, ve, we );
  getForce( t,f,g, A11,A12,A21,A22, xe,ve );
  
  return 0;
}


// ============================================================================================
// Compute the errors.
//
// The number of errors, names and values are returned.
//
// ============================================================================================
int TestRigidBody::
getErrors( int & numErr, aString *& errorNames, RealArray & err )
{

  RealArray xCM(3), vCM(3), rotation(3,3), w(3);
  
  body.getCoordinates( t, xCM, vCM,rotation,w );

  RealArray xe(3),ve(3),we(3);
  getExactSolution( t, xe, ve, we );

  Range R=3;
  
  numErr=3;
  if( errorNames==NULL )
    errorNames = new aString [numErr];
  errorNames[0]="x-err";
  errorNames[1]="v-err";
  errorNames[2]="w-err";

  err.redim(numErr);
  err(0)=max(fabs(xCM-xe));
  err(1)=max(fabs(vCM-ve));
  err(2)=max(fabs(w-we));

  if( testProblem==freeRotation1 ||
      testProblem==freeRotation2 ||
      testProblem==freeRotation3 )
  {
    // NOTE: This is the solution for wHat(t) in the rotating frame, wHat = E^T w 
    //    wHat1 = A cos(alpha*t) - C sigma*sin(alpha*t)
    //    wHat2 = C cos(alpha*t) + A sigma sin(alpha*t)
    //    wHat3 = wHat3(0)
    //
    //    A = wHat1(0), C=wHat2(0)
    //    alpha = abs( wHat3(0)*(I3-I1)/I1 )
    //    sigma = sgn(wHat3(0)*(I3-I1)/I1)

    RealArray e(3,3);
    body.getAxesOfInertia( t,e );

    RealArray wHat(3);
    for( int j=0; j<3; j++ )
      wHat(j) = e(0,j)*w(0)+e(1,j)*w(1)+e(2,j)*w(2);
    
    err(2)=max(fabs(wHat-we));
  }
  
}



// ======================================================================================================
//  Solve the rigid body equations
// ======================================================================================================
int TestRigidBody::
solve(GenericGraphicsInterface & gi)
{

  PlotStuffParameters psp;
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  psp.set(GI_TOP_LABEL,"time 0");  // set title
  
  char buff[180];

  CrossSectionMapping map;
  map.setGridDimensions(axis1,31);
  map.setGridDimensions(axis2,41);

  if( plotOption )
    PlotIt::plot(gi,map,psp);

  RealArray xBound(2,3);
  real xMin=REAL_MAX, xMax=-REAL_MAX;
  for( int axis=0; axis<map.getRangeDimension(); axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      Bound b  = map.getRangeBound(side,axis);
      if( b.isFinite() )
      {
	xMin=min(xMin,(real)b);
	xMax=max(xMax,(real)b);
      }
    }
  }
  // printf("xMin=%e xMax=%e \n",xMin,xMax);
  
  xBound(Start,Range(0,2))=xMin;
  xBound(End  ,Range(0,2))=xMax;
  
  psp.set(GI_USE_PLOT_BOUNDS,true);  // use the region defined by the plot bounds
  psp.set(GI_PLOT_BOUNDS,xBound); // set plot bounds


  // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
  // can rotate/scale and shift any Mapping
  MatrixTransform transform(map);

  numberOfDimensions=map.getRangeDimension();
  
  t=0.;

  transform.reset();

  body.reset();
  
  body.setTimeSteppingMethod( method,orderOfAccuracy );

  initialConditions();
      
  // Indicate whether we include added mass matrices.
  body.includeAddedMass(addedMass);
  RealArray A11(3,3), A12(3,3), A21(3,3), A22(3,3);
  A11=0.;  A12=0.; A21=0.; A22=0.;

    
  int numberOfSteps=int(tFinal/dt+1.5); 
  dt = tFinal/(numberOfSteps-1.);

  real nextTimeToPlot=tPlot;

  const int numberOfComponents=18;
  RealArray solution;
  if( saveMatlabFile )
  {
    const int maxNumberToSave = int(tFinal/tPlot+2.5);
    solution.redim(maxNumberToSave,numberOfComponents+1); // save t here also
  }
    
  RealArray xCM(3), vCM(3);  // holds centre of mass: position, velocity
  RealArray f(3), g(3);      // force and torque
  RealArray w(3);
      
  RealArray r(3,3);
  RealArray xNew(3);
  

  xCM=xCM0;
  vCM=vCM0;

  // Supply forces at t=-dt
  if( orderOfAccuracy>3 )
  {
    real tp = t-dt;
    getExactForcing( tp, f , g, A11,A12,A21,A22 );
    body.setPastTimeForcing( tp, f , g, A11,A12,A21,A22 );
  }
  
//   if( false )
//   {
//     getForce( t,f,g, A11,A12,A21,A22, xCM,vCM );
//     for(int step=1; step<=numberOfSteps; step++) 
//     {
//       body.integrate( t,f,g, t+dt, A11,A12,A21,A22, xNew,r  );
//       t+=dt;
//       if( (step % 100) == 0 )
//       {
// 	printF("step=%i\n",step);
// 	checkArrayIDs(sPrintF("body.integrate: step=%i",step) ); 
//       }
//     }
//     OV_ABORT("finish for now");
//   }
  

  int numberSaved=0;  // for matlab file
    
  for(int step=1; step<=numberOfSteps; step++) 
  {
    // we should output at the start of the step?
    if( t > tFinal -dt*.01 )
      break;

    getForce( t,f,g, A11,A12,A21,A22, xCM,vCM );

    if( addedMass )
      body.integrate( t,f,g, t+dt, A11,A12,A21,A22, xNew,r  );
    else
      body.integrate( t,f,g, t+dt, xNew,r  );

    // int numberOfCorrections= step==1 ? 2 : 1;
    int numberOfCorrections= 1;
    for( int correction=0; correction<numberOfCorrections; correction++ )
    {
      body.getVelocity( t+dt,vCM );
      getForce( t+dt,f,g, A11,A12,A21,A22, xNew,vCM );
      if( addedMass )
	body.correct( t+dt,f,g, A11,A12,A21,A22, xNew,r  );
      else
	body.correct( t+dt,f,g, xNew,r  );
    }
    
    body.getAngularVelocities( t+dt,w );
    body.getVelocity( t+dt,vCM );
      
    // body.getPosition( t+dt,xNew,r );
      
    if( plotBody )
    {
      transform.shift(-xCM(0),-xCM(1),-xCM(2));
      transform.rotate( r );
      transform.shift(xNew(0),xNew(1),xNew(2));
    }
    
    xCM=xNew;
    t+=dt;

    // Draw the mapping
    if( plotBody )
    {
      psp.set(GI_TOP_LABEL,sPrintF(buff,"Step %i, x=(%4.2f,%4.2f,%4.2f)",step,
				   xCM(0),xCM(1),xCM(2)));
      //             omega(0)*180./Pi,omega(1)*180./Pi,omega(1)*180./Pi));  // set title

      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      gi.erase();
      PlotIt::plot(gi,transform,psp);
      gi.redraw(true);   // force a redraw
    }


    if( t>nextTimeToPlot-.5*dt || t>tFinal-dt*.5 )
    {
      nextTimeToPlot+=tPlot;
      if( nextTimeToPlot > tFinal )
        nextTimeToPlot=tFinal;

      // print errors:
      output( testProblem, step );
	  

      if( saveMatlabFile && t>nextTimeToPlot-.5*dt )
      {
	int i=numberSaved;
	  
	assert( i<solution.getBound(0) );
	  
	solution(i,0)=t;

	solution(i,1)=xCM(0);
	solution(i,2)=xCM(1);
	solution(i,3)=xCM(2);

	solution(i,4)=vCM(0);
	solution(i,5)=vCM(1);
	solution(i,6)=vCM(2);

	solution(i,7)=w(0);
	solution(i,8)=w(1);
	solution(i,9)=w(2);
	  
	RealArray e(3,3);
	body.getAxesOfInertia( t,e );
	  
	solution(i,10)=e(0,0);
	solution(i,11)=e(1,0);
	solution(i,12)=e(2,0);
			    			    
	solution(i,13)=e(0,1);
	solution(i,14)=e(1,1);
	solution(i,15)=e(2,1);
			    			    
	solution(i,16)=e(0,2);
	solution(i,17)=e(1,2);
	solution(i,18)=e(2,2);

	numberSaved++;
	  
      }

    }
	
    if( (step % 100) == 0 )
    {
      printF("check array IDs: step=%i\n",step);
      checkArrayIDs(sPrintF("solve: step=%i",step) ); 
    }
    

  } // end for ( step )  (time steps)

  if( saveMatlabFile )
  {
      
    FILE *matlabFile=NULL;
    aString fileName="rigidBody.m";
    // gi.inputString(fileName,sPrintF(answer,
    //     "Enter the name of the matlab file (default=%s)\n",(const char*)fileName));

    matlabFile = fopen((const char*)fileName,"w" ); 
    
    const int numPerLine=5;
    fprintf(matlabFile,"tc=[");
    for( int i=0; i<numberSaved; i++ ) 
    {
      fprintf(matlabFile,"%17.10e ",solution(i,0));
      if( (i % numPerLine)==numPerLine-1 ) fprintf(matlabFile,"...\n");
    }
    fprintf(matlabFile,"];\n");


    fprintf(matlabFile,"yc=[");
    for( int i=0; i<numberSaved; i++ ) 
    {
      for( int j=1; j<=numberOfComponents; j++ )
      {
	fprintf(matlabFile,"%17.10e ",solution(i,j));
	// if( (i % numPerLine)==numPerLine-1 ) fprintf(matlabFile,"...\n");
      }
      fprintf(matlabFile," ; \n");  // end this row  solution(1:nc,i)
    }
      
    fprintf(matlabFile,"];\n");
      
    fclose(matlabFile);
  
    printf("Saved file %s for matlab\n",(const char*)fileName);
      
  }

  return 0;

}


int 
main(int argc, char *argv[]) 
{
  Mapping::debug=0;

  Overture::start(argc,argv);  // initialize Overture and A++/P++

  printF("Usage: trb -cmd=<command file> -noplot -cfl=<> -tFinal=<> -tPlot=<> -dt0=<> -debug=<> ... \n"
         "  -test=[trig| free1|free2|free3|fallingSphere] -addedMass=[0|1] -orderOfAccuracy=[1-4] ...\n"
         "  -conv=[0|1] -numResolutions=<> -plotBody=[0|1] \n");

  TestRigidBody trb;

  int & debug = trb.debug;

  int & plotOption=trb.plotOption;
  bool & saveMatlabFile=trb.saveMatlabFile;  // save results to a matlab file.
  bool & addedMass= trb.addedMass;
  int & orderOfAccuracy= trb.orderOfAccuracy;
  int & plotBody= trb.plotBody;

  RigidBodyMotion::TimeSteppingMethodEnum & method = trb.method;
  method=RigidBodyMotion::implicitRungeKutta;

  
  real &cfl=trb.cfl, &tFinal=trb.tFinal, &tPlot=trb.tPlot;
  real & dt0 = trb.dt0;

  int conv=0;
  int numResolutions=2;  // number of resolutions for convergence tests.

  testProblem=constantX0Acceleration;

  aString commandFileName="";

  aString outputFileName="trb.tex";
  FILE *outputFile=NULL;

  char buff[180];
  int len=0;
  if( argc > 1 )
  { // look at arguments for "-noplot" or "-cfl=<value>"
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" )
        plotOption=false;
      else if( len=line.matches("-cfl=") )
      {
        sScanF(line(len,line.length()-1),"%e",&cfl);
	printF("cfl = %6.2f\n",cfl);
      }
      else if( len=line.matches("-tFinal=") )
      {
        sScanF(line(len,line.length()-1),"%e",&tFinal);
	printF("tFinal = %6.2f\n",tFinal);
      }
      else if( len=line.matches("-tPlot=") )
      {
        sScanF(line(len,line.length()-1),"%e",&tPlot);
	printF("tPlot = %6.3f\n",tPlot);
      }
      else if( len=line.matches("-dt0=") )
      {
        sScanF(line(len,line.length()-1),"%e",&dt0);
	printF("dt0 = %8.2e\n",dt0);
      }
      else if( len=line.matches("-debug=") )
      {
        sScanF(line(len,line.length()-1),"%i",&debug);
	printF("debug = %i\n",debug);
        RigidBodyMotion::debug=debug;
      }
      else if( line==("-test=trig") )
      {
        testProblem=trigonometricMotion;
      }
      else if( line==("-test=free1") )
      {
        testProblem=freeRotation1;
      }
      else if( line==("-test=free2") )
      {
        testProblem=freeRotation2;
      }
      else if( line==("-test=free3") )
      {
        testProblem=freeRotation3;
      }
      else if( line==("-test=fallingSphere") )
      {
        testProblem=fallingSphere;
      }
      else if( len=line.matches("-addedMass=") )
      {
        sScanF(line(len,line.length()-1),"%i",&addedMass);
	printF("addedMass = %i\n",addedMass);
      }
      else if( len=line.matches("-orderOfAccuracy=") )
      {
        sScanF(line(len,line.length()-1),"%i",&orderOfAccuracy);
	printF("orderOfAccuracy = %i\n",orderOfAccuracy);
      }
      else if( len=line.matches("-conv=") )
      {
        sScanF(line(len,line.length()-1),"%i",&conv);
	printF("conv = %i\n",conv);
      }
      else if( len=line.matches("-plotBody=") )
      {
        sScanF(line(len,line.length()-1),"%i",&plotBody);
	printF("plotBody = %i\n",plotBody);
      }
      else if( len=line.matches("-numResolutions=") )
      {
        sScanF(line(len,line.length()-1),"%i",&numResolutions);
	printF("numResolutions = %i\n",numResolutions);
      }
      else if( len=line.matches("-cmd=") )
      {
        commandFileName=line(len,line.length()-1);
        printF("trb: reading commands from file [%s]\n",(const char*)commandFileName);
      }
    }
  }

  PlotStuff gi(plotOption,"test rigid body");

  // By default start saving the command file called "cgins.cmd"
  aString logFile="trb.cmd";
  gi.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("read command file =%s\n",(const char*)commandFileName);
    gi.readCommandFile(commandFileName);
  }


  aString answer;

  GUIState dialog;

  dialog.setWindowTitle("Rigid Body Test");
  dialog.setExitCommand("exit", "exit");

  aString opCommand1[] = {"general motion",
                          "trigonometric motion",
                          "free rotation 1",
                          "free rotation 2",
                          "free rotation 3",
			  "constant x acceleration",
			  "constant y acceleration",
			  "constant z acceleration",
			  "constant omega_x acceleration",
			  "constant omega_y acceleration",
			  "constant omega_z acceleration",
			  "x sine acceleration",
			  "y sine acceleration",
			  "z sine acceleration",
			  "exponential v",
			  "falling sphere",
			  ""};

  dialog.setOptionMenuColumns(1);
  dialog.addOptionMenu( "Type:", opCommand1, opCommand1, testProblem );

  aString opCommand2[] = {"leapFrogTrapezoidal",
                          "improvedEuler",
                          "implicitRungeKutta",
			  ""};

  dialog.setOptionMenuColumns(1);
  dialog.addOptionMenu( "Method:", opCommand2, opCommand2, method );

  aString cmds[] = {"solve",
                    "convergence rate",
                    "leak check",
                    "exit",
		    ""};

  int numberOfPushButtons=3;  // number of entries in cmds
  int numRows=numberOfPushButtons; // (numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  aString tbCommands[] = {"added mass",
                          "plot body",
			  ""};
  int tbState[10];
  tbState[0] = addedMass;
  tbState[1] = plotBody;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


  const int numberOfTextStrings=15;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textLabels[nt] = "tFinal:"; 
  sPrintF(textStrings[nt],"%g",tFinal);  nt++; 

  textLabels[nt] = "dt0:"; 
  sPrintF(textStrings[nt],"%g",dt0);  nt++; 

  textLabels[nt] = "tPlot:"; 
  sPrintF(textStrings[nt],"%g",tPlot);  nt++; 

  textLabels[nt] = "cfl:"; 
  sPrintF(textStrings[nt],"%g",cfl);  nt++; 

  textLabels[nt] = "mass:"; 
  sPrintF(textStrings[nt],"%g",trb.mass);  nt++; 

  textLabels[nt] = "order of accuracy:"; 
  sPrintF(textStrings[nt],"%i",orderOfAccuracy);  nt++; 

  textLabels[nt] = "numResolutions:"; 
  sPrintF(textStrings[nt],"%i",numResolutions);  nt++; 

  real newtonTol=1.e-5;
  textLabels[nt] = "newtonTol:"; 
  sPrintF(textStrings[nt],"%8.2e",newtonTol);  nt++; 

  textLabels[nt] = "debug:"; 
  sPrintF(textStrings[nt],"%i",debug);  nt++; 

  textLabels[nt] = "output file:"; 
  sPrintF(textStrings[nt],"%s",(const char*)outputFileName);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  gi.pushGUI(dialog);


  for(;;)
  {
    
    gi.getAnswer(answer,"");  //  testProblem = (TestProblemEnum)ps.getMenuItem(menu,answer,"Choose a test");

    if( answer=="exit" )
    {
      break;
    }
    else if( dialog.getTextValue(answer,"tFinal:","%e",tFinal) ){} //
    else if( dialog.getTextValue(answer,"tPlot:","%e",tPlot) ){} //
    else if( dialog.getTextValue(answer,"dt0:","%e",dt0) ){} //
    else if( dialog.getTextValue(answer,"cfl:","%e",cfl) ){} //
    else if( dialog.getTextValue(answer,"debug:","%i",debug) ){ RigidBodyMotion::debug=debug; } //
    else if( dialog.getTextValue(answer,"mass:","%e",trb.mass) ){} //
    else if( dialog.getTextValue(answer,"order of accuracy:","%i",orderOfAccuracy) ){} //
    else if( dialog.getTextValue(answer,"numResolutions:","%i",numResolutions) ){} //
    else if( dialog.getTextValue(answer,"output file:","%s",outputFileName) ){} //
    else if( dialog.getTextValue(answer,"newtonTol:","%e",newtonTol) )
    {
      trb.body.setNewtonTolerance( newtonTol );
    }
    else if( dialog.getToggleValue(answer,"added mass",addedMass) ){} //
    else if( dialog.getToggleValue(answer,"plot body",plotBody) ){} //
    else if( answer=="general motion"  ||
	     answer=="trigonometric motion"  ||
	     answer=="free rotation 1"  ||
	     answer=="free rotation 2"  ||
	     answer=="free rotation 3"  ||
	     answer=="constant x acceleration"  ||
	     answer=="constant y acceleration"  ||
	     answer=="constant z acceleration"  ||
	     answer=="constant omega_x acceleration"  ||
	     answer=="constant omega_y acceleration"  ||
	     answer=="constant omega_z acceleration"  ||
	     answer=="x sine acceleration"  ||
	     answer=="y sine acceleration"  ||
	     answer=="z sine acceleration"  ||
	     answer=="exponential v"  ||
	     answer=="falling sphere"  )
    {
      if( answer=="general motion" )
	testProblem=generalMotion;
      else if( answer=="trigonometric motion" )
	testProblem=trigonometricMotion;
      else if( answer=="free rotation 1" )
      {
	testProblem=freeRotation1;
	trb.freeRotationAxis=0;
      }
      else if( answer=="free rotation 2" )
      {
	testProblem=freeRotation2;
	trb.freeRotationAxis=1;
      }
      else if( answer=="free rotation 3" )
      {
	testProblem=freeRotation3;
	trb.freeRotationAxis=2;
      }
      else if( answer=="constant x acceleration" )
	testProblem=constantX0Acceleration;
      else if( answer=="constant y acceleration" )
	testProblem=constantX1Acceleration;
      else if( answer=="constant z acceleration" )
	testProblem=constantX2Acceleration;
      else if( answer=="constant omega_x acceleration" )
	testProblem=constantW0Acceleration;
      else if( answer=="constant omega_y acceleration" )
	testProblem=constantW1Acceleration;
      else if( answer=="constant omega_z acceleration" )
	testProblem=constantW2Acceleration;
      else if( answer=="x sine acceleration" )
	testProblem=sinX0Acceleration;
      else if( answer=="y sine acceleration" )
	testProblem=sinX1Acceleration;
      else if( answer=="z sine acceleration" )
	testProblem=sinX2Acceleration;
      else if( answer=="exponential v" )
	testProblem=exponentialV;
      else if( answer=="falling sphere" )
	testProblem=fallingSphere;

      printF("testProblem=%i\n",(int)testProblem);

    }
    else if( answer=="leapFrogTrapezoidal" ||
             answer=="improvedEuler" ||
	     answer=="implicitRungeKutta" )
    {
      if( answer=="leapFrogTrapezoidal" )
	method=RigidBodyMotion::leapFrogTrapezoidal;
      else if( answer=="improvedEuler" )
	method=RigidBodyMotion::improvedEuler;
      else if( answer=="implicitRungeKutta" )
	method=RigidBodyMotion::implicitRungeKutta;
      else
      {
	OV_ABORT("ERROR");
      }
    }
    else if( answer=="leak check" )
    {
      int numberOfSteps=1000;
      RealArray a(3,3), b(3,3), c;
      RealArray Omegak(3,3), Ak(3,3), omegavk(3), rk(6), vvk(3), vv0(3), A11(3,3),A12(3,3), fvnp1(3);
      RealArray omegav0(3), A21(3,3), A22(3,3), gvnp1(3);
      Range R=3;
      real mass=.1, adt=.1;
      
      for(int step=1; step<=numberOfSteps; step++) 
      {
	// ok c = mult( a,b );
	// ok b = trans( a );
	
	// r(R) = omegavk + 5.*( -mult(Omegak,mult(Ak,omegavk)) );
        


        // rk(R) = mass*(vvk-vv0) - adt*( -mult(A11,vvk) - mult(A12,omegavk) + fvnp1 );

        // NO LEAK: 
	rk(R) = mult(Ak,evaluate(omegavk-omegav0)); 

        // LEAK:
	// rk(R) = mult(Ak,omegavk-omegav0) - adt*( -mult(Omegak,mult(Ak,omegavk)) -mult(A21,vvk) - mult(A22,omegavk) + gvnp1 );

        // LEAK:
	// rk(R+3) = mult(Ak,omegavk-omegav0) - adt*( -mult(Omegak,mult(Ak,omegavk)) -mult(A21,vvk) - mult(A22,omegavk) + gvnp1 );
	
	if( (step % 100) == 0 )
	{
	  printF("step=%i\n",step);
	  checkArrayIDs(sPrintF("leak check : step=%i",step) ); 
	}
      }
      

    }
    else if( answer=="solve" )
    {
      // ------------ Solve the Newton-Euler Equations ----------------

      trb.solve(gi);
      

    }
    else if( answer=="convergence rate" )
    {
      // ---- Solve and compute Convergence Rates -------

      int numErr;
      aString *errorName=NULL;
      RealArray err, maxErr;
      RealArray errors, dtv(numResolutions);

      real dtStart = trb.dt0;
      
      for( int res=0; res<numResolutions; res++ )
      {
        // half the time step:        
	trb.dt0=dtStart/pow(2.,res);
        dtv(res)=trb.dt0;

        trb.solve(gi);
        trb.getErrors( numErr,errorName,err ); // return the number, names and errors (which depends on the test)
	
        if( maxErr.getLength(0)==0 )
	  maxErr.redim(numResolutions,numErr);

	for( int j=0; j<numErr; j++ )
	  maxErr(res,j)=err(j);

      }
      
      RealArray rate(numErr);
      convergenceRate(dtv,maxErr,rate);
      for( int j=0; j<numErr; j++ )
      {
	if( isnan(rate(j)) )
	  rate(j)=0.;
      }
      

      aString solverName=trb.body.getTimeSteppingMethodName();
      aString testName=trb.testName;

      if( outputFile==NULL )
      {
        outputFile=fopen((const char*)outputFileName,"w");
      }
      
      for( int io=0; io<2; io++ )
      {
	FILE *file= io==0 ? stdout : outputFile;  

	// Output results as a Latex table

	fprintf(file,"\\begin{figure}[hbt]\\tableFont %% you should set \\tableFont to \\footnotesize or other size\n");
	fprintf(file,"\\begin{center}\n");
	fprintf(file,"\\begin{tabular}{|l|");
	for( int j=0; j<numErr; j++ )
	  fprintf(file,"c|c|");
	fprintf(file,"} \\hline \n");
	fprintf(file,"\\multicolumn{%d}{|c|}{Rigid body, %s, %s}     \\\\ \\hline\n",1+2*numErr,(const char*)solverName,
		(const char*)testName);
	// \dt  & %s  & r & %s  & r & %s  & r \\ \hline
	fprintf(file,"$\\dt$    ");
	for( int j=0; j<numErr; j++ )
	  fprintf(file,"&  %s   &   r   ",(const char*)errorName[j]);

	fprintf(file,"\\\\ \\hline\n"); 
	for( int i=0; i<numResolutions; i++ )
	{
	  //  dt  & err & ratio  & 4.0{e-4} & ratio    \\ \hline
	  fprintf(file,"%8.6f ",dtv(i));
	  for( int j=0; j<numErr; j++ )
	  {
	    real ratio = maxErr(max(i-1,0),j)/maxErr(i,j); // ratio==1 for i=1
	    if( isnan(ratio) )
	      ratio=0.;
	  
	    if( i==0 )
	      fprintf(file,"& %8.2e &       ",maxErr(i,j));
	    else
	      fprintf(file,"& %8.2e & %4.1f  ",maxErr(i,j),ratio);
	  }
	  fprintf(file,"  \\\\ \\hline\n");
	}

	//   rate  &   $1.99$  &       & $2.01$   &        \\ \hline
	fprintf(file," rate    ");
	for( int j=0; j<numErr; j++ )
	  fprintf(file,"& %5.2f   &       ",rate(j));
      
	fprintf(file,"   \\\\ \\hline\n");
	fprintf(file,"\\end{tabular}\n");
	fprintf(file,"\\caption{Newton-Euler Equations: Scheme=%s, test=%s, Max-norm errors at $t=%4.1f$, mass=%8.2e, addedMass=%d }\n",
		(const char*)solverName,(const char*)testName,trb.tFinal,trb.mass,trb.addedMass);
	fprintf(file,"\\label{tab:Test%s_Scheme%s}\n",(const char*)testName,(const char*)solverName);
	fprintf(file,"\\end{center}\n");
	fprintf(file,"\\end{figure} \n"); 
	
      } // end for io
      printF("LaTex output written to file [%s]\n",(const char*)outputFileName);
      
      delete [] errorName;

      trb.dt0=dtStart;  // reset 

    }
    else
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }

  } // for(;;)
  
  gi.popGUI(); // restore the previous GUI

  Overture::finish(); 
  return 0;
}
