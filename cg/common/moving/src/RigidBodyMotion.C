// #define BOUNDS_CHECK

#include "RigidBodyMotion.h"
#include "GenericGraphicsInterface.h"
#include "display.h"
#include "TimeFunction.h"

int RigidBodyMotion::debug = 0 ;
  

//\begin{>RigidBodyMotionInclude.tex}{\subsection{constructor}} 
RigidBodyMotion::
RigidBodyMotion(int numberOfDimensions_ /* = 3 */)
  : R(0,2)
// =======================================================================================
// /Description:
//    Class for keeping track of the position and orientation of a rigid body moving
// under the influence of forces and torques.
//
// /numberOfDimensions (input):  Indicate whether we are in 2D or 3d. NOTE that this 
//   class basically treats all bodies as 3D (so all input/output is for 3d bodies) but
//   a 2D body will be forced to lie in the x-y plane.
//
// /General Usage:
// \begin{itemize}
//  \item Build a RigidBodyMotion object.
//  \item set the properties, mass and moments of inertia, interactively or using setProperties.
//  \item assign initial conditions (setInitialConditions)
//  \item repeatly call integrate to advance the solution; integrate will return the position
//     of the centre of mass and a rotation matrix that can be used with the MatrixTransform Mapping
//   class, for example, to rotate a Mapping.
// \end{itemize}
// /Background:
// The equations of motion in the standard cartesian reference frame are
// \begin{align*}
//   M {d \vv \over dt} &=  \Fv \\
//   {d  \hv\over dt} &= \sum I_i (\dot{\omega}_i \ev_i+ \omega_i \dot{\ev}_i) = \Gv \\
//   \dot{\ev_i} &= \omegav\times\ev_i \qquad (\ev_i\cdot\ev_i=1, ~~\ev_i\cdot\dot{\ev}_i=0)
// \end{align*}
// or
// \begin{align*}
//   M {d \vv \over dt} &=  \Fv \\
//   I_i \dot{\omega}_i -(I_{i+1}-I_{i+2})\omega_{i+1}\omega_{i+2} &= \Gv\cdot\ev_i \\
//   \dot{\ev_i} &= \omegav\times\ev_i 
// \end{align*}
// where the subscripts on $I$ and $\omega$ are to be taken modulo 3 and 
// where $\ev_i$ are the principal axes of inertia and  $\omega_i$ are the angular velocities about
// the axes of inertia. 
// 
// In this form of the equations we integrate the motion of the principle axes, $\ev_i(t)$ over time along with
// the position of the center of mass.
// The rotation matrix that must be applied to the original body to rotate it to time t
//  is simply $E(t)E^{-1}(0)$ where $E$ is the matrix
// with columns being $\ev_i$,
// \[
//    R(t) = E(t) E^{-1}(0) = \begin{bmatrix} \ev_0(t) & \ev_1(t) & \ev_2(t) \end{bmatrix}
//                           \begin{bmatrix} \ev_0^T(0) \\ \ev_1^T(0) \\ \ev_2^T(0) \end{bmatrix}
// \]
// /numberOfDimensions\_ (input) : 2 or 3 .
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  mass=1.;
  numberOfDimensions=numberOfDimensions_;
  mI.redim(3);
  mI=1.;
  density=-1.; // this means the density is not known
  

  timeSteppingMethod=leapFrogTrapezoidal;
  damping=0.;

  if( false )
  { // try this for light particles:
    timeSteppingMethod=improvedEuler; 
    damping=0.;
  }
  
  numberOfSteps=-1;
  current=-1;
  maximumNumberToSave=5;
  numberSaved=0;

  initialConditionsGiven=0;
  
  positionConstraint=positionHasNoConstraint;
  rotationConstraint=rotationHasNoConstraint;
  constraintValues.redim(15);
  constraintValues=0.;

  time.redim(maximumNumberToSave);  time=0.;
  x0.redim(3);                      x0=0.;
  x.redim(3,maximumNumberToSave);   x=0.;
  v.redim(3,maximumNumberToSave);   v=0.;
  v0.redim(3);                      v0=0.;
  f.redim(3,maximumNumberToSave);   f=0.;
  g.redim(3,maximumNumberToSave);   g=0.;

  mI.redim(3);                      mI=1.;
  w.redim(3,maximumNumberToSave);   w=0.;
  w0.redim(3);                      w0=0.;
  e.redim(3,3,maximumNumberToSave); e=0.;   e(0,0,0)=e(1,1,0)=e(2,2,0)=1.;
 
  e0.redim(3,3);                    e0=0.;  e0(0,0)=e0(1,1)=e0(2,2)=1.;
  
  bodyForceCoeff.redim(3,4);  bodyForceCoeff=0.;   // body force coefficients
  bodyTorqueCoeff.redim(3,4); bodyTorqueCoeff=0.;  // body torque coefficients

  // We keep track if the correction steps have converged (N.B. for "light" rigid bodies for e.g.)
  relaxCorrectionSteps=false;
  correctionHasConverged=true;
  maximumRelativeCorrection=0.;


  correctionAbsoluteToleranceForce=1.e-10;
  correctionRelativeToleranceForce=1.e-2;
  correctionRelaxationParameterForce=.2;

  correctionAbsoluteToleranceTorque=1.e-10;
  correctionRelativeToleranceTorque=1.e-2;
  correctionRelaxationParameterTorque=.2;

  twilightZone=false;
  twilightZoneType=trigonometricTwilightZone;

  bodyForceType=timePolynomialBodyForce;

  // Set the order of accuracy (currently only works with the the implicit RK methods)
  if( !dbase.has_key("orderOfAccuracy") ) dbase.put<int>("orderOfAccuracy",2);

  if( !dbase.has_key("includeAddedMass") ) dbase.put<bool>("includeAddedMass",false);

  if( !dbase.has_key("toleranceNewton") ) dbase.put<real>("toleranceNewton");
  dbase.get<real>("toleranceNewton")=1.e-5;

  if( !dbase.has_key("numberOfPastTimeValues") ) dbase.put<int>("numberOfPastTimeValues");
  dbase.get<int>("numberOfPastTimeValues")=0;

  // For added mass cases we may want to extrapolate in time in the predictor instead of using the
  // equations of motion:
  if( !dbase.has_key("useExtrapolationInPredictor") ) dbase.put<bool>("useExtrapolationInPredictor",false);
  if( !dbase.has_key("orderOfExtrapolationPredictor") ) dbase.put<int>("orderOfExtrapolationPredictor");
  dbase.get<int>("orderOfExtrapolationPredictor")=-1;  // -1 means match to order of accuracy
  
  // For added mass cases we may want to return the acceleration (and angular acceleration) not computed
  // using the equations of motion but rather finite differences of the velocity (or angular velocity)
  if( !dbase.has_key("accelerationComputedByDifferencingVelocity") ) 
    dbase.put<bool>("accelerationComputedByDifferencingVelocity",false);


  logFile=NULL;

}

RigidBodyMotion::
~RigidBodyMotion()
{
  if( logFile!=NULL )
    fclose(logFile);
}

// // ================================================================================================
// /// \brief Initialize the class. This is a protected routine.
// // ================================================================================================
// int
// initialize()
// {


//   return 0;
// }


// =================================================================================================
// ================= Matrix Utility Routines =======================================================
// =================================================================================================


/// \brief return the product if two matrices.
RealArray 
mult( const RealArray & a, const RealArray & b )
{
  // arguments should not be temporaries -- or else there is a leak
  assert( !a.isTemporary() && !b.isTemporary() );  

  // c(m,n) = a(m,l)*b(l,n)
  const int m=a.getLength(0), l=a.getLength(1), n=b.getLength(1);
  if( l!=b.getLength(0) )
  {
    printF("ERROR:mult: these matrices cannot be multipled -- dimensions are wrong.\n");
    display(a,"a");
    display(b,"b");
    
    OV_ABORT("ERROR");
  }
  // assert( a.getBase(0)==0 && a.getBase(1)==0 );
  // assert( b.getBase(0)==0 && b.getBase(1)==0 );
  const int aBase0=a.getBase(0), aBase1=a.getBase(1);
  const int bBase0=b.getBase(0), bBase1=b.getBase(1);
  
  
  RealArray c(m,n);
  for( int i=0; i<m; i++ )
  {
    for( int j=0; j<n; j++ )
    {
      real t=0;
      for( int k=0; k<l; k++ )
	t=t+a(i+aBase0,k+aBase1)*b(k+bBase0,j+bBase1);
      c(i,j)=t;
    }
  }
  return c;
}

/// \brief return the transpose of a matrix
RealArray 
trans( const RealArray &a )
{
  const int m=a.getLength(0), n=a.getLength(1);
  // assert( a.getBase(0)==0 && a.getBase(1)==0 );
  const int aBase0=a.getBase(0), aBase1=a.getBase(1);
  
  RealArray c(n,m);
  for( int i=0; i<m; i++ )
  {
    for( int j=0; j<n; j++ )
    {
      c(j,i)=a(i+aBase0,j+aBase1);
    }
  }
  return c;
}

/// \brief return the dot product of two arrays
real
dot( const RealArray &a, const RealArray &b )
{
  const int m=a.getLength(0);
  if( m!=b.getLength(0) )
  {
    printF("ERROR:dot: the input arrays are not the same length.\n");
    OV_ABORT("ERROR");
  }
  // assert( a.getBase(0)==0 && a.getBase(1)==0 );
  // assert( b.getBase(0)==0 && b.getBase(1)==0 );
  const int aBase0=a.getBase(0), aBase1=a.getBase(1);
  const int bBase0=b.getBase(0), bBase1=b.getBase(1);

  real t=0.;
  for( int i=0; i<m; i++ )
  {
    t+= a(i+aBase0,aBase1)*b(i+bBase0,bBase1);
  }
  return t;
}

/// \brief Return the matrix which represents the cross product of w with another vector, [ w X ]
RealArray
getCrossProductMatrix( const RealArray & w )
{
  const int n=w.getLength(0);
  assert( w.getBase(0)==0 );
  
  RealArray W(n,n);
  W(0,0)=   0.;  W(0,1)=-w(2); W(0,2)= w(1);
  W(1,0)= w(2);  W(1,1)=   0.; W(1,2)=-w(0);
  W(2,0)=-w(1);  W(2,1)= w(0); W(2,2)=   0.;
  

  return W;
}

#define DGECO EXTERN_C_NAME(dgeco)
#define SGECO EXTERN_C_NAME(sgeco)
#define DGESL EXTERN_C_NAME(dgesl)
#define SGESL EXTERN_C_NAME(sgesl)

#ifdef OV_USE_DOUBLE
#define GECO DGECO
#define GESL DGESL
#else
#define GECO SGECO
#define GESL SGESL
#endif
extern "C"
{
void SGECO( real & b, const int & nbd, const int & nb, int & ipvt,real & rcond, real & work );

void DGECO( real & b, const int & nbd, const int & nb, int & ipvt,real & rcond, real & work );

void SGESL(real & a,const int & lda,const int & n,int & ipvt, real & b, int & job);

void DGESL(real & a,const int & lda,const int & n,int & ipvt, real & b, int & job);
  
}

/// \brief solve the matrix equation A*x = b and return x. 
RealArray 
solve( const RealArray & a, const RealArray & b )
{
  const int m=a.getLength(0);
  if( m!=a.getLength(1) )
  {
    printF("solve:ERROR: the array a is not square!\n");
    OV_ABORT("ERROR");
  }
  if( m!=b.getLength(0) )
  {
    printF("ERROR:solve: the array b is not the correct dimension!\n");
    OV_ABORT("ERROR");
  }

  real rcond;
  int job=0;
  real *work = new real [m];
  int *ipvt = new int [m];

  RealArray af; // Make a copy of a since the factorization is store here
  af=a;
  
  GECO( af(0,0),m,m,ipvt[0],rcond,work[0] ); // factor 
  if( rcond==0. )
  {
    printf("solve:ERROR: the condition number is zero! \n");
    display(a,"Here is the matrix a","%9.3e ");  
    OV_ABORT("ERROR");
  }
  RealArray x;
  x=b;
  GESL( af(0,0),m,m,ipvt[0],x(0),job);     // solve

  delete [] work;
  delete [] ipvt;

  return x;
}

// ===========================================================================================
// ===========================================================================================
// ===========================================================================================


// ================================================================================================
/// \brief Choose the time stepping method. Optionally set the order of accuracy.
/// \param method (input) : use this time stepping method.
/// \param orderOfAccuracy (input) : optionally choose the order of accuracy. 
///          Currently only applies to implicit Runge Kutta.
// ================================================================================================
int RigidBodyMotion::
setTimeSteppingMethod( const TimeSteppingMethodEnum method, int orderOfAccuracy /* =defaultOrderOfAccuracy */ )
{
  timeSteppingMethod=method;
  if( orderOfAccuracy!=defaultOrderOfAccuracy )
  {
    dbase.get<int>("orderOfAccuracy")=orderOfAccuracy;
  }
  return 0;
}

// ===============================================================================================
/// \brief Set the convergence tolerance for the Newton iteration of implicit solvers.
/// \details If the Newton is converging quadratically then the actual error in the Newton
///  iteration should be tol*tol. Thus setting tol=1.e-5 will likely give an error of 1.e-10.
// ===============================================================================================
int RigidBodyMotion::
setNewtonTolerance( real tol )
{
  dbase.get<real>("toleranceNewton")=tol;
  return 0;
}


// ===============================================================================================
/// \brief Indicate whether added mass matrices will be used (and provided by the user).
/// \param trueOrFalse (input) : set to true if added mass matrices will be provided.
// ===============================================================================================
int RigidBodyMotion::
includeAddedMass( bool trueOrFalse /* = true */ )
{
  dbase.get<bool>("includeAddedMass")=trueOrFalse;
  return 0;
}


// ===============================================================================================
/// \brief Return true if the added mass matrices are being used.
// ===============================================================================================
bool RigidBodyMotion::
useAddedMass() const
{
  return dbase.get<bool>("includeAddedMass");
}



// =======================================================================================================
// \brief Return the name of the time stepping method as a string.
// =======================================================================================================
aString RigidBodyMotion::
getTimeSteppingMethodName() const
{
  aString name;
  if( timeSteppingMethod==leapFrogTrapezoidal )
  {
    name="leapFrogTrap";
  }
  else if( timeSteppingMethod==improvedEuler )
  {
    name="improvedEuler";
  }
  else if( timeSteppingMethod==implicitRungeKutta )
  {
    name=sPrintF("DIRK%i",dbase.get<int>("orderOfAccuracy"));
  }
  else
  {
    name="Unknown";
  }

  return name;
}


// =======================================================================================================
/// \brief: Evaluate the body force and torque.
/// \param t (input) : evaluate at this time.
/// \param bodyForce, bodyTorque (output) : body force and boy torque at time t
// =======================================================================================================
int RigidBodyMotion::
getBodyForces( const real t, RealArray & bodyForce, RealArray & bodyTorque ) const
{
  if( bodyForceType==timePolynomialBodyForce )
  {
    bodyForce(R) = bodyForceCoeff(R,0) + t*( bodyForceCoeff(R,1) + t*( bodyForceCoeff(R,2) 
								       + t*(bodyForceCoeff(R,3))));
    bodyTorque(R) = bodyTorqueCoeff(R,0) + t*( bodyTorqueCoeff(R,1) + t*( bodyTorqueCoeff(R,2) 
									  + t*(bodyTorqueCoeff(R,3))));
  }
  else if( bodyForceType==timeFunctionBodyForce )
  {
    // Body forces and torques are defined by time functions
    bodyForce(R)=0.;
    bodyTorque(R)=0.;
    
    real f=0.,g=0.;
    for( int axis=0; axis<3; axis++ )
    {
      const char* timeFunctionName = ( axis==0 ? "bodyForceX" : axis==1 ? "bodyForceY" : "bodyForceZ" );
      if( dbase.has_key(timeFunctionName) )
      {
	TimeFunction & timeFunction = dbase.get<TimeFunction>(timeFunctionName);
	timeFunction.eval(t,f );
	bodyForce(axis)=f;
	printF("RigidBodyMotion::getBodyForces: %s : t=%9.3e, force=%9.3e\n",timeFunctionName,t,f);
	
      }
    }
    for( int axis=0; axis<3; axis++ )
    {
      const char* timeFunctionName = ( axis==0 ? "bodyTorqueX" : axis==1 ? "bodyTorqueY" : "bodyTorqueZ" );
      if( dbase.has_key(timeFunctionName) )
      {
	TimeFunction & timeFunction = dbase.get<TimeFunction>(timeFunctionName);
	timeFunction.eval(t,g );
        bodyTorque(axis)=g;
	printF("RigidBodyMotion::getBodyForces: %s : t=%9.3e, torque=%9.3e\n",timeFunctionName,t,g);
      }
    }
    

  }
  else
  {
    OV_ABORT("RigidBodyMotion::getBodyForces:ERROR: unknown bodyForceType");
  }
  

  return 0;
}


// =======================================================================================================
/// \brief: Evaluate the added mass matrices at time t. 
/// \details Interpolate from the sequence of saved values in time.
/// \param t (input) : evaluate matrices at this time.
/// \param A11 , A12 , A21, A22 (output) : added mass matrices.
// =======================================================================================================
int RigidBodyMotion::
getAddedMassMatrices( const real t, RealArray & A11 , RealArray & A12 , RealArray & A21, RealArray & A22 ) const
{
  const bool includeAddedMass = dbase.get<bool>("includeAddedMass");
  if( includeAddedMass )
  {
    // Check that the AddedMass exists. (created on the first call to integrate).
    if( !dbase.has_key("AddedMass" ) )
    {
      printF("RigidBodyMotion::getAddedMassMatrices:WARNING: The AddedMass matrices do not exist.\n"
             "You need to pass the added mass matrices to integrate if the added mass option is on.\n");
      A11=0.; A12=0.; A21=0.; A22=0.;
      return 0.;
    }

    
    const RealArray & AddedMass = dbase.get<RealArray>("AddedMass");

    assert( current>=0 );
    const int previous = (current-1+maximumNumberToSave) % maximumNumberToSave;
    const int next = (current+1) % maximumNumberToSave;

    RealArray *AM[4] = { &A11, &A12, &A21, &A22 }; // make pointers to the 4 added mass matrices for convenience

    assert( numberSaved>=1 );

    const int orderOfAccuracy = dbase.get<int>("orderOfAccuracy");

    // The user may have supplied values of the forcings at negative times.
    const int numberOfForcingsSaved = numberSaved + dbase.get<int>("numberOfPastTimeValues");

    // Linear interpolation in time:
    if( orderOfAccuracy<=2 || numberOfForcingsSaved<=1 ) // Note numberSaved does not include solution "next"
    {
      // Linear interpolation in time:
      real t1=time(next), t2=time(current);
      real c1 = (t-t2)/(t1-t2);
      real c2 = (t-t1)/(t2-t1);
      for( int k=0; k<4; k++ )
      {
	*AM[k] = c1*AddedMass(R,R,k,next) + c2*AddedMass(R,R,k,current);
      }

    }
    else if( orderOfAccuracy==3 || numberOfForcingsSaved<=2 )
    {
      // Lagrange interpolation in time
      real t1=time(next), t2=time(current), t3=time(previous);
      real c1 = ( (t-t2)*(t-t3) )/( (t1-t2)*(t1-t3) );
      real c2 = ( (t-t3)*(t-t1) )/( (t2-t3)*(t2-t1) );
      real c3 = ( (t-t1)*(t-t2) )/( (t3-t1)*(t3-t2) );

      for( int k=0; k<4; k++ )
      {
	*AM[k] = c1*AddedMass(R,R,k,next) + c2*AddedMass(R,R,k,current) + c3*AddedMass(R,R,k,previous);
      }

    }

    else if( orderOfAccuracy==4 )
    {
      // Lagrange interpolation in time, use 4 time levels
      const int prev2 = (current-2+maximumNumberToSave) % maximumNumberToSave;

      real t1=time(next), t2=time(current), t3=time(previous), t4=time(prev2);
      real c1 = ( (t-t2)*(t-t3)*(t-t4) )/( (t1-t2)*(t1-t3)*(t1-t4) );
      real c2 = ( (t-t3)*(t-t4)*(t-t1) )/( (t2-t3)*(t2-t4)*(t2-t1) );
      real c3 = ( (t-t4)*(t-t1)*(t-t2) )/( (t3-t4)*(t3-t1)*(t3-t2) );
      real c4 = ( (t-t1)*(t-t2)*(t-t3) )/( (t4-t1)*(t4-t2)*(t4-t3) );
      
      for( int k=0; k<4; k++ )
      {
	*AM[k] = c1*AddedMass(R,R,k,next) + c2*AddedMass(R,R,k,current) + c3*AddedMass(R,R,k,previous) + c4*AddedMass(R,R,k,prev2);
      }
      
    }
    else
    {
      printF("RigidBodyMotion::getAddedMassMatrices:ERROR: interpolation for orderOfAccuracy=%i not implemented\n",orderOfAccuracy);
      OV_ABORT("ERROR");
    }



  }
  else
  {
    A11=0.;
    A12=0.;
    A21=0.;
    A22=0.;
  }
  

  return 0;
}


// =================================================================================
/// \brief Return the maximum relative change in the moving grid correction scheme.
///    This is usually only an issue for "light" bodies. 
// =================================================================================
real RigidBodyMotion::
getMaximumRelativeCorrection()
{
  return maximumRelativeCorrection; 
}

// =================================================================================
/// \brief Return true if the correction steps for moving grids have converged.
///    This is usually only an issue for "light" bodies. 
// =================================================================================
bool RigidBodyMotion::
getCorrectionHasConverged()
{
  return correctionHasConverged;
}


// ================================================================================================
/// \brief Reset the rigid body to it's initial state. (e.g. remove saved solutions etc.)
// ================================================================================================
int RigidBodyMotion::
reset()
{
  numberOfSteps=-1;
  current=-1;
  maximumNumberToSave=5;
  numberSaved=0;
  dbase.get<int>("numberOfPastTimeValues")=0;
  
  initialConditionsGiven=0;

  time=0.;
  x0=0.;
  x=0.;
  v=0.;
  v0=0.;
  f=0.;
  g=0.;

  mI=1.;
  w=0.;
  w0=0.;
  e=0.;   e(0,0,0)=e(1,1,0)=e(2,2,0)=1.;
 
  e0=0.;  e0(0,0)=e0(1,1)=e0(2,2)=1.;
  
  bodyForceCoeff=0.;   
  bodyTorqueCoeff=0.;   

}


//\begin{>>RigidBodyMotionInclude.tex}{\subsection{setProperties}} 
int RigidBodyMotion:: 
setProperties(real totalMass, 
	      const RealArray & momentOfInertia, 
	      const int numberOfDimensions_ /* = 3 */ )
// =======================================================================================
// /Description:
//    Specify properties of the rigid body.
//
// /totalMass (input) : mass of the body
// /momentOfInertia(0:2) (input) : moments of inertial. In 2D only momentsOfInertia(2) is required.
//
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  numberOfDimensions=numberOfDimensions_;
  setMass(totalMass);
  mI(R)=momentOfInertia(R);
  
  return 0;
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{centerOfMassHasBeenInitialized}}
bool RigidBodyMotion::
centerOfMassHasBeenInitialized() const 
// =======================================================================================
// /Description:
//   Return true if the centre of mass has been given initial values.
//
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{ 
  return (initialConditionsGiven % 2) == 1; 
} 

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{massHasBeenInitialized}}
bool RigidBodyMotion::
massHasBeenInitialized() const 
// =======================================================================================
// /Description:
//   Return true if the mass has been given an initial value.
//
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{ 
  return ((initialConditionsGiven/2) % 2) == 1; 
} 

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{setInitialCenterOfMass}} 
int RigidBodyMotion::
setInitialCentreOfMass( const RealArray & xInitial )
// =======================================================================================
// /Description:
//   Supply the inital conditions for integrating the equations of motion. 
// 
// /xInitial(0:2) (input) : inital values for centre of mass.
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  if( (initialConditionsGiven % 2)==0 )
    initialConditionsGiven+=1;  // this means the center of mass has been specified.
  
  x0(R)=xInitial(R);
  x(R,0)=x0(R);
  return 0;
}


//\begin{>>RigidBodyMotionInclude.tex}{\subsection{setInitialConditions}} 
int RigidBodyMotion::
setInitialConditions(real t0 /* = 0. */, 
                     const RealArray & xInitial /* = Overture::nullRealArray() */ , 
                     const RealArray & vInitial /* = Overture::nullRealArray() */ , 
                     const RealArray & wInitial /* = Overture::nullRealArray() */ ,
                     const RealArray & axesOfInertia /* = Overture::nullRealArray() */ )
// =======================================================================================
// /Description:
//   Supply the inital conditions for integrating the equations of motion. 
// 
// /t0 (input) : inital time.
// /x0(0:2), vInitial(0:2), wInitial(0:2) (input) : inital values for centre of mass, velocity of the centre of mass
// and the angular velocities.
// /axesOfInertia (input) : (not required in 2d) if this array is dimensioned to be at least 3x3 then it will contain
// the initial orientation of the axes of inertia, axesOfInertia(0:2,i) will be the components of the
//  i'th axes of inertia. This also represents the rotation matrix that transforms the coordinate axes
//  into the axes of inertia.
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  numberOfSteps=0;

  current=-1;
  numberSaved=0;
  time(0)=t0;

  if( xInitial.getLength(0)>=3 )
  {
    if( (initialConditionsGiven % 2)==0 )
      initialConditionsGiven+=1;  // this means the center of mass has been specified.
    x0(R)=xInitial(R);
  }
  else
    x0=0.;

  x(R,0)=x0(R);
  if( vInitial.getLength(0)>=3 ) 
    v0(R)=vInitial(R);
  else
    v0(R)=0.;
  
  v(R,0)=v0(R);
  if( wInitial.getLength(0)>=3 ) 
    w0=wInitial(R);
  else
    w0=0.;
  w(R,0)=w0;
  if( numberOfDimensions==3 && axesOfInertia.getLength(0)>2 && axesOfInertia.getLength(1)>2 )
  {
    e0(R,R)=axesOfInertia(R,R);
    e(R,R,0)=e0;
  }
  else
  {
    // default is the identity matrix
    e0=0.;  e0(0,0)=e0(1,1)=e0(2,2)=1.;
    e=0.;   e(0,0,0)=e(1,1,0)=e(2,2,0)=1.;
  }
  
  
  
  return 0;
}


//\begin{>>RigidBodyMotionInclude.tex}{\subsection{momentumTransfer}} 
int RigidBodyMotion::
momentumTransfer( real t0, RealArray & vNew )
// =======================================================================================
// /Description:
//    Redefine the velocity after a collision.
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  // const int previous = (current-1+maximumNumberToSave) % maximumNumberToSave;
  const int next     = (current+1+maximumNumberToSave) % maximumNumberToSave;

//  if( time(current)==t0 )
{
  v(R,next)=vNew(R);
  const real dt=t0-time(current);

  x(R,current)=x(R,next)-dt*vNew(R);
  if( debug & 1 )
    printF("RigidBodyMotion::momentumTransfer: t0=%e, time(next)=%e, dt=%8.2e\n",t0,time(current),dt);
}
  


return 0;
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{setPositionConstraint}}
int RigidBodyMotion:: 
setPositionConstraint( PositionConstraintEnum constraintType, const RealArray & values )
// ==================================================================================
// /Description:
//    Define a constraint on the position (usually the centre of mass) of the rigid body.
// The position can be contrained to lie in a plane, line along a line or be fixed.
//
// \begin{verbatim}
//   enum PositionConstraintEnum
//   {
//     positionHasNoConstraint=0,
//     positionConstrainedToAPlane,
//     positionConstrainedToALine,
//     positionIsFixed
//   };
// \end{verbatim}
// 
// /constraint (input) : type of constraint (from the enum PositionConstraintEnum)
// /values (input) : For positionConstrainedToAPlane, (v11,v12,v13)=values(0:2) 
//   and (v21,v22,v23)=values (3:5) define the two orthonormal vectors of the constraint plane.
//  For positionConstrainedToALine, (v11,v12,v13)=values(0:2) defines the tangent to the constraining line.
//
// /Notes:
//    More generally we could allow the constraint to apply to a point other than the centre of mass.
// For example, the rigid body could be constrained to rotate about a given point. This option needs
// to be added -- constraintValues(0:2) is allocated to save this other "position"
//
//\end{RigidBodyMotionInclude.tex}  
// ==================================================================================
{
  positionConstraint=constraintType;

  if( positionConstraint==positionConstrainedToAPlane )
  {
    if( constraintValues.getLength(0)<9 )
      constraintValues.resize(9); 

    constraintValues(Range(3,5))=values(Range(0,2));
    constraintValues(Range(6,8))=values(Range(3,5));
  }
  else if( positionConstraint== positionConstrainedToALine )
  {
    if( constraintValues.getLength(0)<6 )
      constraintValues.resize(6); 

    constraintValues.resize(max(9,constraintValues.getLength(0)));
    constraintValues(Range(3,5))=values(Range(0,2));
  }
  
  return 0;
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{setRotationConstraint}}
int RigidBodyMotion:: 
setRotationConstraint( RotationConstraintEnum constraintType, const RealArray & values )
// ==================================================================================
// /Description:
//    Define a constraint on the rotation of the rigid body.
// The rotation can be contrained to lie in a plane, line along a line or be fixed.
//
// \begin{verbatim}
//   enum RotationConstraintEnum
//   {
//     rotationHasNoConstraint=0,
//     rotationConstrainedToAPlane,
//     rotationConstrainedToALine,
//     rotationIsFixed
//   };
// \end{verbatim}
//
// /constraint (input) : type of constraint (from the enum RotationConstraintEnum)
// /values (input) : For rotationConstrainedToAPlane, (v11,v12,v13)=values(0:2) 
//   and (v21,v22,v23)=values (3:5) define the two orthonormal vectors of the constraint plane.
//  For rotationConstrainedToALine, (v11,v12,v13)=values(0:2) defines the tangent to the constraining line.
////
//\end{RigidBodyMotionInclude.tex}  
// ==================================================================================
{
  rotationConstraint=constraintType;

  if( rotationConstraint==rotationConstrainedToAPlane )
  {
    if( constraintValues.getLength(0)<15 )
      constraintValues.resize(15); 

    constraintValues(Range(9,11))=values(Range(0,2));
    constraintValues(Range(12,14))=values(Range(3,5));
  }
  else if( rotationConstraint== rotationConstrainedToALine )
  {
    if( constraintValues.getLength(0)<12 )
      constraintValues.resize(12); 

    constraintValues.resize(max(9,constraintValues.getLength(0)));
    constraintValues(Range(9,11))=values(Range(0,2));
  }

  return 0;
}

int RigidBodyMotion:: 
applyConstraints( const int step, const ConstraintApplicationEnum option )
// ==================================================================================
// /Description:
//    This is a protected routine -- apply a constraint to the forces, torques, positions or rotations.
// The position and rotation constraints are imposed at two stages -- the forces and torques are
// adjusted at one stage (option=applyConstraintsToForces) while the actual positions and 
// rotation angles are adjusted
// at the second stage (option=applyConstraintsToPosition)
// 
// /step : apply constraints at this step.
// /option: option=applyConstraintsToForces : apply constraints to the force and torques
//          option=applyConstraintsToPosition : apply constraints to the positions and angular variables
// 
//\end{RigidBodyMotionInclude.tex}  
// ==================================================================================  
{
  if( positionConstraint==positionHasNoConstraint && 
      rotationConstraint==rotationHasNoConstraint )
  {
    return 0; // no constraints
  }
  
  if( option==applyConstraintsToForces )
  {
    // ****************************************************
    // *** apply constraints to the forcing and torques ***
    // ****************************************************

    if( positionConstraint==positionConstrainedToALine )
    {
      // the force should only have a component along the line
      RealArray vector(3);  vector=constraintValues(Range(3,5));
       
      real dot = sum(f(R,step)*vector(R));  // this assumes the tangent to the line is normalized
      f(R,step) = dot*vector(R);
      
      // adjust the torque -- should be parallel to the line as well
      dot = sum(g(R,step)*vector(R));  // this assumes the tangent to the line is normalized
      g(R,step) = dot*vector(R);
      

    }
    else if( positionConstraint==positionConstrainedToAPlane )
    {
      if( numberOfDimensions==3 )
      {
	RealArray v1(3), v2(3), v3(3); 
        v1=constraintValues(Range(3,5)); v2=constraintValues(Range(6,8));
        // v3 is orthogonal to v1 and b2
        v3(0) = v1(1)*v2(2)-v1(2)*v2(1);
        v3(1) = v1(2)*v2(0)-v1(0)*v2(2);
        v3(2) = v1(0)*v2(1)-v1(1)*v2(0);
	
	// subtract off the component of f in the direction of v3 -- this will project f onto the
        // plane spanned by (v1,v2)
	real dot= sum(f(R,step)*v3(R)); 
	f(R,step) -=  dot*v3(R);
      }
    }
    else if( positionConstraint==positionIsFixed )
    {
      f(R,step)=0.;
    }
    
    if( rotationConstraint==rotationIsFixed )
    {
      g(R,step)=0.;
    }
    else if( rotationConstraint==rotationConstrainedToALine )
    {
      // the force should only have a component along the line
      RealArray vector(3);  vector=constraintValues(Range(9,11));

      real dot = sum(g(R,step)*vector(R));  // this assumes the tangent to the line is normalized
      g(R,step) = dot*vector(R);
    
    }
    else if( rotationConstraint==rotationConstrainedToAPlane )
    {
      if( numberOfDimensions==3 )
      {
	RealArray v1(3), v2(3), v3(3); 
        v1=constraintValues(Range(9,11)); v2=constraintValues(Range(12,14));
        // v3 is orthogonal to v1 and b2
        v3(0) = v1(1)*v2(2)-v1(2)*v2(1);
        v3(1) = v1(2)*v2(0)-v1(0)*v2(2);
        v3(2) = v1(0)*v2(1)-v1(1)*v2(0);
	
	// subtract off the component of g in the direction of v3 -- this will project g onto the
        // plane spanned by (v1,v2)
	real dot= sum(g(R,step)*v3(R)); 
	g(R,step) -=  dot*v3(R);
      }
    }
    
  }
  else if( option==applyConstraintsToPosition )
  {

    // *****************************************
    // *** apply constraints to the position ***
    // *****************************************

    if( positionConstraint==positionConstrainedToALine )
    {
      // the position should lie on a line through the initial xCM
      RealArray vector(3);  vector=constraintValues(Range(3,5));
       
      real dot = sum((x(R,step)-x0(R))*vector(R));  // this assumes the tangent to the line is normalized

      x(R,step) = x0(R) + dot*vector(R);
      
      // printf("RigidBodyMotion:applyConstraint: apply constraint to x at step=%i\n",step);
      
    }
    else if( positionConstraint==positionConstrainedToAPlane )
    {
      if( numberOfDimensions==3 )
      {
	RealArray v1(3), v2(3), v3(3); 
        v1=constraintValues(Range(3,5)); v2=constraintValues(Range(6,8));
        // v3 is orthogonal to v1 and b2
        v3(0) = v1(1)*v2(2)-v1(2)*v2(1);
        v3(1) = v1(2)*v2(0)-v1(0)*v2(2);
        v3(2) = v1(0)*v2(1)-v1(1)*v2(0);
	
	// subtract off the component of x-x0 in the direction of v3 -- this will project x onto the
        // plane x0 + Span(v1,v2)
        //   b = [(x-x0).v3]v3
        //   xNew = x - b
        // => (xNew-x0).v3 = 0
	real dot= sum((x(R,step)-x0(R))*v3(R)); 
	x(R,step) -= dot*v3(R);
      }
    }
    else if( positionConstraint==positionIsFixed )
    {
      x(R,step)=x0(R);
    }
    
    if( rotationConstraint==rotationIsFixed )
    {
      w(R,step)=0.;
    }
    else if( rotationConstraint==rotationConstrainedToALine )
    {
      // the force should only have a component along the line
      RealArray vector(3);  vector=constraintValues(Range(9,11));

      real dot = sum((w(R,step)-w0(R))*vector(R));  // this assumes the tangent to the line is normalized
      w(R,step) = w0(R) + dot*vector(R);
    
    }
    else if( rotationConstraint==rotationConstrainedToAPlane )
    {
      if( numberOfDimensions==3 )
      {
	RealArray v1(3), v2(3), v3(3); 
        v1=constraintValues(Range(9,11)); v2=constraintValues(Range(12,14));
        // v3 is orthogonal to v1 and b2
        v3(0) = v1(1)*v2(2)-v1(2)*v2(1);
        v3(1) = v1(2)*v2(0)-v1(0)*v2(2);
        v3(2) = v1(0)*v2(1)-v1(1)*v2(0);
	
	// subtract off the component of g in the direction of v3 -- this will project g onto the
        // plane spanned by (v1,v2)
	real dot= sum((w(R,step)-w0(R))*v3(R)); 
	w(R,step) -=  dot*v3(R);
      }
    }
  }
  else if( option==applyConstraintsToVelocity )
  {
    Overture::abort("applyConstraintsToVelocity: error"); 
  }
  else
  {
    Overture::abort("error");
  }

  
  return 0;
}


// ==================================================================================
/// \brief Supply forcing at "negative" times for startup.
/// \details The fourth-order scheme requires values at t=-dt.
/// \param t (input) : a past time value (e.g. t=-dt)
/// \force, torque (input) : force and torque at time t
// ==================================================================================
int RigidBodyMotion::
setPastTimeForcing( real t, const RealArray & force, const RealArray & torque )
{
  return setPastTimeForcing(t,force,torque,Overture::nullRealArray(),Overture::nullRealArray(),
                            Overture::nullRealArray(),Overture::nullRealArray() );
}

// ==================================================================================
/// \brief Supply forcing at "negative" times for startup.
/// \details The fourth-order scheme requires values at t=-dt.
/// \param t (input) : a past time value (e.g. t=-dt)
/// \param force, torque (input) : force and torque at time t
/// \param A11,A12,A21,A22 (input) : added mass matrices. (only used if added mass matrices are being used)
// ==================================================================================
int RigidBodyMotion::
setPastTimeForcing( real t, const RealArray & force, const RealArray & torque,
		    const RealArray & A11, const RealArray & A12, const RealArray & A21, const RealArray & A22  )
{
  int & numberOfPastTimeValues = dbase.get<int>("numberOfPastTimeValues");

  numberOfPastTimeValues++;
  if( numberOfPastTimeValues != 1 )
  {
    printF("RigidBodyMotion::setPastTimeForcing: ERROR: not expecting multiple past time values. FIX ME\n");
    OV_ABORT("error");
  }

  if( current != -1 )
  {
    printF("RigidBodyMotion::setPastTimeForcing: ERROR: current!=-1 -- I was not expecting this.\n");
    printF("   Is setPastTimeForcing being called after the first call to integrate?\n");
    OV_ABORT("error");
  }
  

  if( t>=0. )
  {
    printF("RigidBodyMotion::setPastTimeForcing:ERROR: t=%9.3e >= 0.\n",t,0.);
    OV_ABORT("error");
  }
  
  int current0 = 0;
  const int prev = ( current0-1 +maximumNumberToSave) % maximumNumberToSave;

  time(prev)=t;
  
  printF("RigidBodyMotion::setPastTimeForcing: prev=%i, t=%9.3e f=(%8.2e,%8.2e,%8.2e) g=(%8.2e,%8.2e,%8.2e)\n",
	 prev, t, force(0),force(1),force(2),torque(0),torque(1),torque(2) );

  f(R,prev)=force;
  g(R,prev)=torque;

  const bool includeAddedMass = dbase.get<bool>("includeAddedMass");
  if( includeAddedMass )
  {
    // save the Added mass matrices.
    if( !dbase.has_key("AddedMass" ) )
    {
      // Create the AddMass array that holds the time sequence of added mass matrices.

      printF("$$$$$ RigidBodyMotion::setPastTimeForcing: INFO: create AddedMass array... $$$$\n");
      
      dbase.put<RealArray>("AddedMass");
      RealArray & AddedMass = dbase.get<RealArray>("AddedMass");

      AddedMass.redim(R,R,4,maximumNumberToSave);
      AddedMass=0.;
    }
    RealArray & AddedMass = dbase.get<RealArray>("AddedMass");

    AddedMass(R,R,0,prev)=A11;
    AddedMass(R,R,1,prev)=A12;
    AddedMass(R,R,2,prev)=A21;
    AddedMass(R,R,3,prev)=A22;

  }
    

  return 0;
  
}



// ==================================================================================
/// \brief Integrate the equations of motion from time t0 to time t using force and moment
/// information at time t0.
/// 
/// \param t0 (input) : starting time for the time step.
/// \param force(0:2) (input) : components of the total force at time t0
/// \param torque(0:2) (input) : components of the torque (about the center of mass) at time t0.
/// \param t (input) : end time for the time step.
/// \param xCM(0:2) (output) : position of the centre of mass at time t
/// \param rotation(0:2,0:2) (output) : rotation matrix at time t. This matrix will be orthonormal (explicitly enforced
///    by this routine).
///   
// ==================================================================================
int RigidBodyMotion:: 
integrate(real t0, 
          const RealArray & force, 
	  const RealArray & torque,
	  real t, 
          RealArray & xCM, 
	  RealArray & rotation )
{
  return integrate(t0,force,torque,t,
                   Overture::nullRealArray(),
                   Overture::nullRealArray(),
                   Overture::nullRealArray(),
                   Overture::nullRealArray(),
                   xCM, rotation );
}


// ==================================================================================
/// \brief Integrate the equations of motion from time t0 to time t using force and moment
/// information at time t0. This version takes added mass matrices.
/// 
/// \param t0 (input) : starting time for the time step.
/// \param force(0:2) (input) : components of the total force at time t0
/// \param torque(0:2) (input) : components of the torque (about the center of mass) at time t0.
/// \param t (input) : end time for the time step.
/// \param A11, A12, A21, A22 (input) : added matrices of size (0:2,0:2). These are used if the
///    use of added mass matrices has been turned on with a call to includeAddedMass.
///
/// \param xCM(0:2) (output) : position of the centre of mass at time t
/// \param rotation(0:2,0:2) (output) : rotation matrix at time t. This matrix will be orthonormal (explicitly enforced
///    by this routine).
///   
// ==================================================================================
int RigidBodyMotion:: 
integrate(real t0, 
          const RealArray & force, 
	  const RealArray & torque,
	  real t, 
          const RealArray & A11, const RealArray & A12, const RealArray & A21, const RealArray & A22, 
          RealArray & xCM, 
	  RealArray & rotation )
{
  if( numberOfSteps<0 )
  {
    printf("RigidBodyMotion::integrate:WARNING:setInitialConditions should be called before integrate\n");
    numberOfSteps=0;
  }
  
  numberOfSteps++;

  if( logFile!=NULL )
  {
    fPrintF(logFile,"\n--Predictor step %i: t=%9.3e --\n",numberOfSteps,t);
  }

  if( (current>=0 &&  t0<time(current)) || (numberOfSteps==0 && t0<time(0)) )
  {
    printf("RigidBodyMotion::integrate:ERROR: t0=%6.2e < time(current)=%6.2e integrating backwards in time?\n",t0,
	   numberOfSteps==0  ? time(0) : time(current));
    OV_ABORT("error");
  }
  

  current = (current+1) % maximumNumberToSave;
  numberSaved=min(numberSaved+1, maximumNumberToSave);

  const int previous = (current-1+maximumNumberToSave) % maximumNumberToSave;
  const int next     = (current+1) % maximumNumberToSave;


  time(current)=t0;

  const real dt=t-t0;
  time(next)=t;

  RealArray bodyForce(R), bodyTorque(R);
  getBodyForces( t0, bodyForce, bodyTorque ); // body force and torque at the start of the time step

  f(R,current)=force(R) +bodyForce(R);
  g(R,current)=torque(R)+bodyTorque(R);

  applyConstraints( current,applyConstraintsToForces );
  
  const bool includeAddedMass = dbase.get<bool>("includeAddedMass");
  if( includeAddedMass )
  {
    // save the Added mass matrices.
    if( !dbase.has_key("AddedMass" ) )
    {
      // Create the AddMass array that holds the time sequence of added mass matrices.

      printF("$$$$$ RigidBodyMotion::integrate: INFO: create AddedMass array... $$$$\n");
      
      dbase.put<RealArray>("AddedMass");
      RealArray & AddedMass = dbase.get<RealArray>("AddedMass");

      AddedMass.redim(R,R,4,maximumNumberToSave);
      AddedMass=0.;
    }
    RealArray & AddedMass = dbase.get<RealArray>("AddedMass");

    AddedMass(R,R,0,current)=A11;
    AddedMass(R,R,1,current)=A12;
    AddedMass(R,R,2,current)=A21;
    AddedMass(R,R,3,current)=A22;

    // --- adjust the force for the added mass terms
    //   >> we do this on the predictor step since the force will not have been projected yet
    //      IS this how we want to do this ??
    if( logFile!=NULL )
    {
      fPrintF(logFile,"  Input: f=(%9.3e,%9.3e,%9.3e) g=(%9.3e,%9.3e,%9.3e)\n",f(0,current),f(1,current),f(2,current),
	      g(0,current),g(1,current),g(2,current));
    }

    f(R,current) += mult(A11,v(R,current)) + mult(A12,w(R,current));
    g(R,current) += mult(A21,v(R,current)) + mult(A22,w(R,current));
    
    if( logFile!=NULL )
    {
      fPrintF(logFile," Add added mass: f=(%9.3e,%9.3e,%9.3e) g=(%9.3e,%9.3e,%9.3e)\n",f(0,current),f(1,current),f(2,current),
	      g(0,current),g(1,current),g(2,current));
    }

  }

  // --- guess forces at next time ---

  // The user may have supplied values of the forcings at negative times.

  const int & numberOfPastTimeValues = dbase.get<int>("numberOfPastTimeValues");
  
  const int numberOfForcingsSaved = numberSaved + numberOfPastTimeValues;
  if( numberOfForcingsSaved<=1 || timeSteppingMethod==improvedEuler )
  {
    f(R,next)=f(R,current);   
    g(R,next)=g(R,current);
    if( includeAddedMass )
    {
      // guess for added mass at next time
      RealArray & AddedMass = dbase.get<RealArray>("AddedMass");
      for( int k=0; k<4; k++ )
	AddedMass(R,R,k,next)=AddedMass(R,R,k,current);
    }

  }
  else
  {
    if( false && numberOfPastTimeValues>0  )
    {
      printF("numberOfPastTimeValues=%i, t=%9.3e, t(prev)=%9.3e \n", numberOfPastTimeValues,time(current),
	     time(previous));
      
    }
    

    const int orderOfAccuracy = dbase.get<int>("orderOfAccuracy");
    if( orderOfAccuracy<=3  || numberOfForcingsSaved<3 )
    {
      // Linear extrapolation from the current and previous times.
      real dtOld= numberOfForcingsSaved>1 ? time(current)-time(previous) : 0.;
      real dtRatio;
      if( dtOld!=0. )
	dtRatio=dt/dtOld;
      else
	dtRatio=0.;
      real ab1= 1.+dtRatio; // normally 2
      real ab2= -dtRatio;   // normally -1

      f(R,next)=ab1*f(R,current)+ab2*f(R,previous);
      g(R,next)=ab1*g(R,current)+ab2*g(R,previous);

      if( includeAddedMass )
      {
        RealArray & AddedMass = dbase.get<RealArray>("AddedMass");
	for( int k=0; k<4; k++ )
	  AddedMass(R,R,k,next)=ab1*AddedMass(R,R,k,current)+ab2*AddedMass(R,R,k,previous);
      }
    }
    else
    {
      // Fourth-order: use 3 point extrapolation (third-order). With 1 correction step the
      // method should be fourth order. NOTE: 2 correction steps should be used on the first time step.
      // Each correction step increases the order of accuracy until it reaches 4. IS this true?? 

      // Quadratic extrapolation from the current and two previous times.
      const int prev2 = (current-2+maximumNumberToSave) % maximumNumberToSave;
      real t1=time(current), t2=time(previous), t3=time(prev2);
      // printF("integrate: predict f(t+dt) : (t1,t2,t3)=(%9.3e,%9.3e,%9.3e)\n",t1,t2,t3);
      
      real c1 = ( (t-t2)*(t-t3) )/( (t1-t2)*(t1-t3) );
      real c2 = ( (t-t3)*(t-t1) )/( (t2-t3)*(t2-t1) );
      real c3 = ( (t-t1)*(t-t2) )/( (t3-t1)*(t3-t2) );

      f(R,next)= c1*f(R,current) + c2*f(R,previous)+ c3*f(R,prev2);
      g(R,next)= c1*g(R,current) + c2*g(R,previous)+ c3*g(R,prev2);

      if( includeAddedMass )
      {
        RealArray & AddedMass = dbase.get<RealArray>("AddedMass");
	for( int k=0; k<4; k++ )
	{
	  AddedMass(R,R,k,next) = c1*AddedMass(R,R,k,current) + c2*AddedMass(R,R,k,previous) + c3*AddedMass(R,R,k,prev2);
	}
      }
      
    }
    

  }
  
  if( timeSteppingMethod==leapFrogTrapezoidal ||
      timeSteppingMethod==improvedEuler )
  {
    takeStepLeapFrog( t0, dt );
  }
  else if( timeSteppingMethod==implicitRungeKutta )
  {
    // implicit RK -- predictor step
    takeStepImplicitRungeKutta( t0, dt );
  }
  else
  {
    // --- OLD WAY ---
    OV_ABORT("ERROR");

//     printF("RB:integrate:WARNING: old way be used.\n");

//     RealArray vDot(R);
//     vDot(R)=f(R,current)/mass;

//     // NOTE: the v array is not used here in computing x, but is used in the corrector.
//     if( numberOfSteps==2 )
//     {
//       // at t=dt we  recompute v(dt) = v(0)+ ( vDot(0)*dt + vDot(dt)*dt )*.5
//       v(R,current)=v(R,previous)+( v(R,current)-v(R,previous) + dt*vDot(R) )*.5;
//     }
//     if( false )
//     {
//       printf("RB:integrate: current=%i numberOfSteps=%i v0=(%8.2e,%8.2e) \n",current,numberOfSteps,
// 	     v0(0),v0(1));
//     }
  
//     if( numberOfSteps==1 )
//     {
//       v(R,next)=v0(R)+dt*vDot(R); // this is only first order but is fixed at second step.
//       x(R,next)=x(R,current)+dt*( v0(R) + .5*dt*vDot(R) );
//     }
//     else
//     {
//       if( timeSteppingMethod==leapFrogTrapezoidal )
//       {
// 	v(R,next)=v(R,previous)+(2.*dt)*vDot(R);  // leap frog
//       }
//       else if( timeSteppingMethod==improvedEuler )
//       {
// 	v(R,next)=v(R,current)+dt*vDot(R);   // forward euler 
//       }
//       else
//       {
// 	Overture::abort("error");
//       }
//       // *wdh* 062306 -- add a damping factor      
//       x(R,next)=2.*x(R,current)-x(R,previous)+ (dt*dt)*( vDot(R) -damping*dt*(x(R,current)-x(R,previous)) );
//     }
  
//     if( numberOfDimensions==2 )
//     {
//       x(2,next)=0.;
//       v(2,next)=0.;
//     }

//     // xCM(R)=x(R,next);

//     RealArray wDot(R), eDot(R,R);
//     int axis;
//     for( axis=0; axis<3; axis++ )
//     {
//       const int axisp1=(axis+1) % 3;
//       const int axisp2=(axis+2) % 3;
//       wDot(axis)=( (mI(axisp1)-mI(axisp2))*w(axisp1,current)*w(axisp2,current) +
// 		   g(0,current)*e(0,axis,current)+
// 		   g(1,current)*e(1,axis,current)+
// 		   g(2,current)*e(2,axis,current) )/mI(axis);
//     }
  
//     if( numberOfSteps==2 )
//     {
//       // at t=dt we  recompute w(dt) = w(0)+ ( wDot(0)*dt + wDot(dt)*dt )*.5
//       w(R,current)=w(R,previous)+( w(R,current)-w(R,previous) + dt*wDot(R) )*.5;
//     }

//     if( numberOfSteps==1 )
//     {
//       w(R,next)=w(R,current)+wDot(R)*dt;  // this is only first order for w(dt), but we fix this at step 2
//     }
//     else
//     {
//       w(R,next)=w(R,previous)+wDot(R)*(2.*dt); 
//     }
  
//     // eDot =  w X e
//     eDot(0,R)=w(1,current)*e(2,R,current)-w(2,current)*e(1,R,current);
//     eDot(1,R)=w(2,current)*e(0,R,current)-w(0,current)*e(2,R,current);
//     eDot(2,R)=w(0,current)*e(1,R,current)-w(1,current)*e(0,R,current);
  

//     if( numberOfSteps==1 )
//     {
//       RealArray eDotDot(3,3);
    
//       eDotDot(0,R)=(wDot(1,current)*   e(2,R,current)-wDot(2,current)*   e(1,R,current)+
// 		    w(1,current)*eDot(2,R,current)-   w(2,current)*eDot(1,R,current));
//       eDotDot(1,R)=(wDot(2,current)*   e(0,R,current)-wDot(0,current)*   e(2,R,current)+
// 		    w(2,current)*eDot(0,R,current)-   w(0,current)*eDot(2,R,current));
//       eDotDot(2,R)=(wDot(0,current)*   e(1,R,current)-wDot(1,current)*   e(0,R,current)+
// 		    w(0,current)*eDot(1,R,current)-   w(1,current)*eDot(0,R,current));
    
//       e(R,R,next)=e(R,R,current)+dt*(eDot(R,R)+.5*dt*eDotDot(R,R));
//     }
//     else
//     {
//       e(R,R,next)=e(R,R,previous)+eDot(R,R)*(2.*dt);
//     }
  
//     if( numberOfDimensions==2 )
//     {
//       // 2D: first 2 axes should remain in the plane.
//       e(2,R,next)=0.;
//       e(R,2,next)=0.;
//       e(2,2,next)=1.;
//     }
  
//     // The matrix e(R,R,next) should be orthonormal
//     // Renormalize and orthogonalize the axes of inertia
//     int n;
//     for( n=0; n<3; n++ )
//     {
//       // make column "n" orthogonal to all previous columns "m"
//       for( int m=0; m<n; m++ )
//       {
// 	real dot=e(0,m,next)*e(0,n,next)+e(1,m,next)*e(1,n,next)+e(2,m,next)*e(2,n,next);
// 	e(R,n,next)-=dot*e(R,m,next);
//       }
//       // normalized column n
//       real normInverse=1./max(REAL_MIN,SQRT( SQR(e(0,n,next))+SQR(e(1,n,next))+SQR(e(2,n,next))));
//       e(R,n,next)*=normInverse;
      
//     }
  

// //     for( n=0; n<3; n++ )
// //       rotation(R,n)=e(R,0,next)*e0(0,n)+e(R,1,next)*e0(1,n)+e(R,2,next)*e0(2,n);

//     applyConstraints( next,applyConstraintsToPosition );
    
  }
  


  if( false )
  {
    printF("integrate: t=%e, current=%i x(next)=(%6.2e,%6.2e,%6.2e), w(current)=(%6.2e,%6.2e,%6.2e)\n",
	   t,current,x(0,next),x(1,next),x(2,next),w(0,current),w(1,current),w(2,current));
  }
  if( debug & 1 )
  {
    printF("RB: integrate: t0=%9.3e, t=%9.3e, current=%i x(next)=(%8.2e,%8.2e,%8.2e), force=(%8.2e,%8.2e,%8.2e)\n"
           "     bodyForce=(%8.2e,%8.2e,%8.2e), f(cur)=(%8.2e,%8.2e,%8.2e), f(next)=(%8.2e,%8.2e,%8.2e)\n"
           "     bodyTorque=(%8.2e,%8.2e,%8.2e), g(cur)=(%8.2e,%8.2e,%8.2e), g(next)=(%8.2e,%8.2e,%8.2e)\n",
	   t0,t,current,x(0,next),x(1,next),x(2,next),force(0),force(1),force(2),
           bodyForce(0),bodyForce(1),bodyForce(2),
           f(0,current),f(1,current),f(2,current),
           f(0,next),f(1,next),f(2,next),
           bodyTorque(0),bodyTorque(1),bodyTorque(2),
           g(0,current),g(1,current),g(2,current),
           g(0,next),g(1,next),g(2,next));
  }
  if( logFile!=NULL )
  {
    fPrintF(logFile," integrate: t0=%9.3e, t=t=%9.3e, x(next)=(%8.2e,%8.2e,%8.2e) v(next)=(%8.2e,%8.2e,%8.2e), cur=%i next=%i\n"
                    "     w(next)=(%8.2e,%8.2e,%8.2e) force=(%8.2e,%8.2e,%8.2e) torque=(%8.2e,%8.2e,%8.2e)\n"
                    "     bodyForce=(%8.2e,%8.2e,%8.2e), bodyTorque=(%8.2e,%8.2e,%8.2e)\n"
                    "     f(cur)=(%8.2e,%8.2e,%8.2e), f(next)=(%8.2e,%8.2e,%8.2e)\n"
                    "     g(cur)=(%8.2e,%8.2e,%8.2e), g(next)=(%8.2e,%8.2e,%8.2e)\n",
	    t0,t,x(0,next),x(1,next),x(2,next), v(0,next),v(1,next),v(2,next),current,next,
            w(0,next),w(1,next),w(2,next),
            force(0),force(1),force(2),torque(0),torque(1),torque(2),
            bodyForce(0),bodyForce(1),bodyForce(2),
            bodyTorque(0),bodyTorque(1),bodyTorque(2),
            f(0,current),f(1,current),f(2,current),f(0,next),f(1,next),f(2,next),
	    g(0,current),g(1,current),g(2,current),g(0,next),g(1,next),g(2,next));
    fflush(logFile);
  }
  
  if( dbase.get<bool>("useExtrapolationInPredictor") )
  {
    // -- Use extrapolation in time to determine x, v, w, E ---

    // Linear extrapolation from the current and previous times.
    if( numberSaved<=2 )
    {
      real dtOld= numberSaved>1 ? time(current)-time(previous) : 0.;
      real dtRatio;
      if( dtOld!=0. )
	dtRatio=dt/dtOld;
      else
	dtRatio=0.;
      real ab1= 1.+dtRatio; // normally 2
      real ab2= -dtRatio;   // normally -1

      x(R,next)  =ab1*x(R,current)  +ab2*x(R,previous);
      v(R,next)  =ab1*v(R,current)  +ab2*v(R,previous);
      w(R,next)  =ab1*w(R,current)  +ab2*w(R,previous);
      e(R,R,next)=ab1*e(R,R,current)+ab2*e(R,R,previous);
    }
    else
    {
      // Lagrange interpolation in time -- should we do higher-order here too?

      const int prev2 = (current-2+maximumNumberToSave) % maximumNumberToSave;
      real t1=time(current), t2=time(previous), t3=time(prev2);
      real c1 = ( (t-t2)*(t-t3) )/( (t1-t2)*(t1-t3) );
      real c2 = ( (t-t3)*(t-t1) )/( (t2-t3)*(t2-t1) );
      real c3 = ( (t-t1)*(t-t2) )/( (t3-t1)*(t3-t2) );

      x(R,next)  = c1*x(R,current)  + c2*x(R,previous)  + c3*x(R,prev2);
      v(R,next)  = c1*v(R,current)  + c2*v(R,previous)  + c3*v(R,prev2);
      w(R,next)  = c1*w(R,current)  + c2*w(R,previous)  + c3*w(R,prev2);
      e(R,R,next)= c1*e(R,R,current)+ c2*e(R,R,previous)+ c3*e(R,R,prev2);

      if( logFile!=NULL )
	fPrintF(logFile," EXTRAP: (t1,t2,t3)=(%9.3e,%9.3e,%9.3e) c1=%8.2e, c2=%8.2e c3=%8.2e\n",t1,t2,t3,c1,c2,c3);
      

    }
    
    if( logFile!=NULL )
    {
      fPrintF(logFile," EXTRAP to t=%9.3e: x(next)=(%11.4e,%11.4e,%11.4e) v(next)=(%11.4e,%11.4e,%11.4e) "
	      "w(next)=(%11.4e,%11.4e,%11.4e)\n",t,x(0,next),x(1,next),x(2,next), 
	      v(0,next),v(1,next),v(2,next),w(0,next),w(1,next),w(2,next));
    }
  }
  

  
  
  
  // --- Return values: 

  xCM(R)=x(R,next);

  for( int n=0; n<3; n++ )
    rotation(R,n)=e(R,0,next)*e0(0,n)+e(R,1,next)*e0(1,n)+e(R,2,next)*e0(2,n);


  return 0;
}

// ==================================================================================
/// \brief Correct the solution at time t using new values for the forces and torques.
/// \details This routine might be called by a predictor-corrector type algorithm.
/// 
/// \param t (input) : time (corresponding to the end of the time step).
/// \param force(0:2) (input) : components of the total force at time t
/// \param torque(0:2) (input) : components of the torque (about the center of mass) at time t
// 
/// \param xCM(0:2) (output) : new positions of the centre of mass at time t
/// \param rotation(0:2,0:2) (output) : rotation matrix at time t. This matrix will be orthonormal (explicitly enforced
///    by this routine).
///   
// ==================================================================================
int RigidBodyMotion:: 
correct(real t, 
	const RealArray & force, 
	const RealArray & torque,
	RealArray & xCM, 
	RealArray & rotation )
{
  return correct( t,force,torque,
		  Overture::nullRealArray(),
		  Overture::nullRealArray(),
		  Overture::nullRealArray(),
		  Overture::nullRealArray(),
                  xCM,rotation);
}



// ==================================================================================
/// \brief Correct the solution at time t using new values for the forces and torques.
/// This version takes added mass matrices.
/// \details This routine might be called by a predictor-corrector type algorithm.
/// 
/// \param t (input) : time (corresponding to the end of the time step).
/// \param force(0:2) (input) : components of the total force at time t
/// \param torque(0:2) (input) : components of the torque (about the center of mass) at time t
/// \param A11, A12, A21, A22 (input) : added matrices of size (0:2,0:2). These are used if the
///    use of added mass matrices has been turned on with a call to includeAddedMass.
/// 
/// \param xCM(0:2) (output) : new positions of the centre of mass at time t
/// \param rotation(0:2,0:2) (output) : rotation matrix at time t. This matrix will be orthonormal (explicitly enforced
///    by this routine).
///   
// ==================================================================================
int RigidBodyMotion:: 
correct(real t, 
	const RealArray & force, 
	const RealArray & torque,
        const RealArray & A11, const RealArray & A12, const RealArray & A21, const RealArray & A22,
	RealArray & xCM, 
	RealArray & rotation )

{
  if( logFile!=NULL )
  {
    fPrintF(logFile,"\n--Correction step %i: t=%9.3e --\n",numberOfSteps,t);
  }
  
  correctionHasConverged=true;
  maximumRelativeCorrection=0.;

  const bool includeAddedMass = dbase.get<bool>("includeAddedMass");
  if( includeAddedMass )
  {
    // Check that the AddedMass matrix has been created.
    if( !dbase.has_key("AddedMass" ) )
    {
      printF("RigidBodyMotion::correct:ERROR: The AddedMass matrix does not exist.\n"
             "You need to pass the added mass matrices to integrate if the added mass option is on.\n");
      OV_ABORT("ERROR");
    }
  }
  const RealArray *AM[4] = { &A11, &A12, &A21, &A22 }; // pointers to Added mass matrices for convenience

  RealArray bodyForce(R), bodyTorque(R);
  getBodyForces( t, bodyForce, bodyTorque ); // body force and torque at end of the time step

  if( numberOfSteps<0 )
  {
    printF("RigidBodyMotion::integrate:WARNING:setInitialConditions should be called before integrate\n");
    numberOfSteps=0;
  }
  else if( numberOfSteps==0 )
  {
    // assign forces for t==0
    f(R,0)=force(R)+bodyForce(R);
    g(R,0)=torque(R)+bodyTorque(R);
    xCM(R)=x(R,0);
    rotation=0.;
    rotation(0,0)=rotation(1,1)=rotation(2,2)=1.;

    if( includeAddedMass )
    {
      RealArray & AddedMass = dbase.get<RealArray>("AddedMass");
      for( int k=0; k<4; k++ )
	AddedMass(R,R,k,0)=*AM[k];
    }
    
    applyConstraints( 0,applyConstraintsToForces );
    return 0;
  }
  
  assert( current>=0 );
  const int next     = (current+1) % maximumNumberToSave;

  if( time(next)!=t )
  {
    printF("RigidBodyMotion::correct:ERROR: t=%6.2e != time(next)=%6.2e\n",t,time(next));
    OV_ABORT("error");
  }
  

  if( timeSteppingMethod==improvedEuler )
  {
    // average force with the predicted value *wdh* 062306
    f(R,next)=.5*( force(R)+bodyForce(R) + f(R,next));
    // f(R,next)=.5*(force(R)+bodyForce(R) + f(R,current));
    g(R,next)=.5*( torque(R)+bodyTorque(R) + g(R,next));
  }
  else
  {
    // -- Leap-frog trapezoidal or Implicit RK --
 
    if( relaxCorrectionSteps )
    {
      
      // --- under-relax the new value for f(t+dt), force(R), with the old value for f(t+dt), f(R,next). ---
      //  -- This seems to work for "light" bodies.
      

      real fNorm = sqrt(sum(f(R,next)*f(R,next)));
      real fDiff = max(fabs(force(R)-f(R,next)));
      real fDiffRelative = fDiff/max(1.e-5,fNorm);  // ****** note 1.e-5 -- FIX ME ---

      real gNorm = sqrt(sum(g(R,next)*g(R,next)));
      real gDiff = max(fabs(torque(R)-g(R,next)));
      real gDiffRelative = gDiff/max(1.e-5,gNorm);  // ****** note 1.e-5 -- FIX ME ---


      real relativeDiff = max(fDiffRelative,gDiffRelative);
      real absoluteDiff = max(fDiff,gDiff);

      maximumRelativeCorrection=max(maximumRelativeCorrection,relativeDiff );  // fix me -- use relative ? 

      // The corrections are assumed to have converged if the relative OR the absolute tolerance has been met:
      bool forceHasConverged = fDiffRelative<correctionRelativeToleranceForce ||
	fDiff < correctionAbsoluteToleranceForce;

      bool torqueHasConverged = gDiffRelative<correctionRelativeToleranceTorque ||
	gDiff < correctionAbsoluteToleranceTorque;

      correctionHasConverged= forceHasConverged && torqueHasConverged;

      const real alpha=correctionRelaxationParameterForce; 
      const real beta =correctionRelaxationParameterTorque; 
      if( debug & 2 )
      {
	printF("RB:correct: relax force : t=%8.2e alpha=%8.2e: fDiff=%8.2e <? %8.2e or fDiff/fNorm=%8.2e <? %8.2e\n"
	       "          : relax torque: t=%8.2e beta =%8.2e: gDiff=%8.2e <? %8.2e or gDiff/gNorm=%8.2e <? %8.2e\n"
	       "           f(old)=(%8.2e,%8.2e,%8.2e) f(new)=(%8.2e,%8.2e,%8.2e), g(new)=(%8.2e,%8.2e,%8.2e)\n",
	       t,alpha,fDiff,correctionAbsoluteToleranceForce,fDiffRelative,correctionRelativeToleranceForce,
               t,beta, gDiff,correctionAbsoluteToleranceTorque,gDiffRelative,correctionRelativeToleranceTorque,
               f(0,next),f(1,next),f(2,next),force(0),force(1),force(2),
	       torque(0),torque(1),torque(2));
      }
      
      f(R,next)=alpha*( force(R)+bodyForce(R)  ) + (1.-alpha)*f(R,next);
    
      g(R,next)= beta*( torque(R)+bodyTorque(R) ) + (1.-beta)*g(R,next);  

    }
    else
    {
      // no force relaxation: just update the force at the new time with the given values
      f(R,next)=force(R)+bodyForce(R);
      g(R,next)=torque(R)+bodyTorque(R);
    }

  }
  
  // Update the added mass matrices at the new time.
  if( includeAddedMass )
  {
    RealArray & AddedMass = dbase.get<RealArray>("AddedMass");
    for( int k=0; k<4; k++ )
      AddedMass(R,R,k,next)=*AM[k];
  }

  applyConstraints( next,applyConstraintsToForces );

  assert( current!=next );

  const real dt=t-time(current);
  const real t0=t-dt;
  
  if( timeSteppingMethod==leapFrogTrapezoidal ||
      timeSteppingMethod==improvedEuler )
  {
    takeStepTrapezoid( t0, dt );
  }
  else if( timeSteppingMethod==implicitRungeKutta )
  {
    // implicit RK -- corrector step
    takeStepImplicitRungeKutta( t0, dt );
  }
  else
  {

    OV_ABORT("ERROR");
    

//     // printF("RB:correct:WARNING: old way be used.\n");


//     v(R,next)=v(R,current) + .5*dt*( f(R,current)/mass + f(R,next)/mass );

//     x(R,next)=x(R,current) + .5*dt*( v(R,next)+v(R,current) );
  
//     RealArray wDot(R), wDotCurrent(R), eDot(R,R), eDotCurrent(R,R);
//     int axis;
//     for( axis=0; axis<3; axis++ )
//     {
//       const int axisp1=(axis+1) % 3;
//       const int axisp2=(axis+2) % 3;
//       wDot(axis)=( (mI(axisp1)-mI(axisp2))*w(axisp1,next)*w(axisp2,next) +
// 		   g(0,next)*e(0,axis,next)+
// 		   g(1,next)*e(1,axis,next)+
// 		   g(2,next)*e(2,axis,next) )/mI(axis);
    
//       wDotCurrent(axis)=( (mI(axisp1)-mI(axisp2))*w(axisp1,current)*w(axisp2,current)+
// 			  g(0,current)*e(0,axis,current)+
// 			  g(1,current)*e(1,axis,current)+ 
// 			  g(2,current)*e(2,axis,current) )/mI(axis);
//     }
  
//     w(R,next)=w(R,current)+.5*dt*( wDot(R)+wDotCurrent(R) );

//     // eDot =  w X e
//     eDot(0,R)=w(1,next)*e(2,R,next)-w(2,next)*e(1,R,next);
//     eDot(1,R)=w(2,next)*e(0,R,next)-w(0,next)*e(2,R,next);
//     eDot(2,R)=w(0,next)*e(1,R,next)-w(1,next)*e(0,R,next);
  
//     eDotCurrent(0,R)=w(1,current)*e(2,R,current)-w(2,current)*e(1,R,current);
//     eDotCurrent(1,R)=w(2,current)*e(0,R,current)-w(0,current)*e(2,R,current);
//     eDotCurrent(2,R)=w(0,current)*e(1,R,current)-w(1,current)*e(0,R,current);
  

//     e(R,R,next)=e(R,R,current)+.5*dt*(eDot(R,R)+eDotCurrent(R,R));

  
//     if( numberOfDimensions==2 )
//     {
//       // 2D: first 2 axes should remain in the plane.
//       e(2,R,next)=0.;
//       e(R,2,next)=0.;
//       e(2,2,next)=1.;
//     }
  
//     // The matrix e(R,R,next) should be orthonormal
//     // Renormalize and orthogonalize the axes of inertia
//     for( int n=0; n<3; n++ )
//     {
//       // make column "n" orthogonal to all previous columns "m"
//       for( int m=0; m<n; m++ )
//       {
// 	real dot=e(0,m,next)*e(0,n,next)+e(1,m,next)*e(1,n,next)+e(2,m,next)*e(2,n,next);
// 	e(R,n,next)-=dot*e(R,m,next);
//       }
//       // normalized column n
//       real normInverse=1./max(REAL_MIN,SQRT( SQR(e(0,n,next))+SQR(e(1,n,next))+SQR(e(2,n,next))));
//       e(R,n,next)*=normInverse;
      
//     }

//     applyConstraints( next,applyConstraintsToPosition );
    
  }
  

  // return values:

  xCM(R)=x(R,next);

  for( int n=0; n<3; n++ )
    rotation(R,n)=e(R,0,next)*e0(0,n)+e(R,1,next)*e0(1,n)+e(R,2,next)*e0(2,n);



  if( debug & 1 )
  {
    printF("RB: correct: t=%e, dt=%e, current=%i x(next)=(%6.2e,%6.2e,%6.2e), f(next)=(%6.2e,%6.2e,%6.2e), g(next)=(%6.2e,%6.2e,%6.2e)\n",
	   t,dt,current,x(0,next),x(1,next),x(2,next),f(0,next),f(1,next),f(2,next),g(0,next),g(1,next),g(2,next));
    printF("  : vCM=(%6.2e,%6.2e,%6.2e), w=(%6.2e,%6.2e,%6.2e)\n",v(0,next),v(1,next),v(2,next),
	   w(0,next),w(1,next),w(2,next));
  }

  if( logFile!=NULL )
  {
    fPrintF(logFile," correct: t=%9.3e, x(next)=(%11.4e,%11.4e,%11.4e), v(next)=(%11.4e,%11.4e,%11.4e), cur=%i, next=%i\n"
                    "     w(next)=(%11.4e,%11.4e,%11.4e), force=(%11.4e,%11.4e,%11.4e), torque=(%11.4e,%11.4e,%11.4e), \n"
	            "     f(next)=(%11.4e,%11.4e,%11.4e), g(next)=(%11.4e,%11.4e,%11.4e)\n\n",
	    t,x(0,next),x(1,next),x(2,next),v(0,next),v(1,next),v(2,next),current,next,
            w(0,next),w(1,next),w(2,next),
            force(0),force(1),force(2), torque(0),torque(1),torque(2),
	    f(0,next),f(1,next),f(2,next),g(0,next),g(1,next),g(2,next)); 

    fflush(logFile);
  }

  return 0;
}



// ============================================================================================
/// \brief Take a time step with the leap-frog (predictor). This is a protected routine.
/// \param t0 : current time.
/// \param dt : time step.
// ============================================================================================
int RigidBodyMotion::
takeStepLeapFrog( const real t0, const real dt )
{
  const bool includeAddedMass = dbase.get<bool>("includeAddedMass");
  if( includeAddedMass )
  {
    printF("RigidBodyMotion::takeStepLeapFrog:ERROR: not implemented for added mass yet\n"
           " You should an implicit RK scheme instead\n");
    OV_ABORT("ERROR");
  }
  
  const real t = t0+dt;

  const int previous = (current-1+maximumNumberToSave) % maximumNumberToSave;
  const int next     = (current+1) % maximumNumberToSave;

  RealArray A(R,R), Ai(R,R), Lambda(R,R), LambdaInverse(R,R), Omega(R,R), ea(R,R);
  Lambda=0.;          // diagonal matrix that holds moment of inertial eigenvalues
  LambdaInverse=0.;   // Inverse of Lambda
  for( int j=0; j<3; j++ )
  {
    Lambda(j,j)=mI(j);
    LambdaInverse(j,j)=1./mI(j);
  }
  
  RealArray fv(R), gv(R);
  if( twilightZone )
  {
    ea = e(R,R,current);
    A  = mult(ea,mult(Lambda,trans(ea)));
    getForceInternal( t0,fv,gv,&A );
  }
  else
  {
    fv=f(R,current);
    gv=g(R,current);
  }
      

  RealArray vDot(R);
  vDot(R)=fv/mass;

  // NOTE: the v array is not used here in computing x, but is used in the corrector.
  if( numberOfSteps==2 )
  {
    // at t=dt we  recompute v(dt) = v(0)+ ( vDot(0)*dt + vDot(dt)*dt )*.5
    v(R,current)=v(R,previous)+( v(R,current)-v(R,previous) + dt*vDot(R) )*.5;
  }
  if( false )
  {
    printf("RigidBodyMotion:takeStepLeapFrog: current=%i numberOfSteps=%i v0=(%8.2e,%8.2e) \n",current,numberOfSteps,
	   v0(0),v0(1));
  }
  
  if( numberOfSteps==1 )
  {
    v(R,next)=v0(R)+dt*vDot(R); // this is only first order but is fixed at second step.
    x(R,next)=x(R,current)+dt*( v0(R) + .5*dt*vDot(R) );
  }
  else
  {
    if( timeSteppingMethod==leapFrogTrapezoidal )
    {
      v(R,next)=v(R,previous)+(2.*dt)*vDot(R);  // leap frog
    }
    else if( timeSteppingMethod==improvedEuler )
    {
      v(R,next)=v(R,current)+dt*vDot(R);   // forward euler 
    }
    else
    {
      Overture::abort("error");
    }
    // *wdh* 062306 -- add a damping factor      
    x(R,next)=2.*x(R,current)-x(R,previous)+ (dt*dt)*( vDot(R) -damping*dt*(x(R,current)-x(R,previous)) );
  }
  
  if( numberOfDimensions==2 )
  {
    x(2,next)=0.;
    v(2,next)=0.;
  }

  // xCM(R)=x(R,next);

  RealArray wDot(R), eDot(R,R);
  

  if( true )
  {
    // new way
    //  h' = g, h =A*w,  A= E*Lambda*E^T, A^{-1} = E*Lambda^{-1}*E^T
    //  A * wDot = - Omega*A*w  + g 

    ea = e(R,R,current);
    A  = mult(ea,mult(Lambda,trans(ea)));
    Ai = mult(ea,mult(LambdaInverse,trans(ea)));  // A^{-1}
    Omega = getCrossProductMatrix( w(R,current) );

    wDot = mult( Ai, evaluate(- mult(Omega,mult(A,w(R,current))) + gv) );
  }
  else
  {
    // old way
    for( int axis=0; axis<3; axis++ )
    {
      const int axisp1=(axis+1) % 3;
      const int axisp2=(axis+2) % 3;
      wDot(axis)=( (mI(axisp1)-mI(axisp2))*w(axisp1,current)*w(axisp2,current) +
		   gv(0)*e(0,axis,current)+
		   gv(1)*e(1,axis,current)+
		   gv(2)*e(2,axis,current) )/mI(axis);
    }
  }
  
  if( numberOfSteps==2 )
  {
    // at t=dt we  recompute w(dt) = w(0)+ ( wDot(0)*dt + wDot(dt)*dt )*.5
    w(R,current)=w(R,previous)+( w(R,current)-w(R,previous) + dt*wDot(R) )*.5;
  }

  if( numberOfSteps==1 )
  {
    w(R,next)=w(R,current)+wDot(R)*dt;  // this is only first order for w(dt), but we fix this at step 2
  }
  else
  {
    w(R,next)=w(R,previous)+wDot(R)*(2.*dt); 
  }
  
  // eDot =  w X e
  eDot(0,R)=w(1,current)*e(2,R,current)-w(2,current)*e(1,R,current);
  eDot(1,R)=w(2,current)*e(0,R,current)-w(0,current)*e(2,R,current);
  eDot(2,R)=w(0,current)*e(1,R,current)-w(1,current)*e(0,R,current);
  

  if( numberOfSteps==1 )
  {
    RealArray eDotDot(3,3);
    
    eDotDot(0,R)=(wDot(1,current)*   e(2,R,current)-wDot(2,current)*   e(1,R,current)+
		  w(1,current)*eDot(2,R,current)-   w(2,current)*eDot(1,R,current));
    eDotDot(1,R)=(wDot(2,current)*   e(0,R,current)-wDot(0,current)*   e(2,R,current)+
		  w(2,current)*eDot(0,R,current)-   w(0,current)*eDot(2,R,current));
    eDotDot(2,R)=(wDot(0,current)*   e(1,R,current)-wDot(1,current)*   e(0,R,current)+
		  w(0,current)*eDot(1,R,current)-   w(1,current)*eDot(0,R,current));
    
    e(R,R,next)=e(R,R,current)+dt*(eDot(R,R)+.5*dt*eDotDot(R,R));
  }
  else
  {
    e(R,R,next)=e(R,R,previous)+eDot(R,R)*(2.*dt);
  }
  
  if( numberOfDimensions==2 )
  {
    // 2D: first 2 axes should remain in the plane.
    e(2,R,next)=0.;
    e(R,2,next)=0.;
    e(2,2,next)=1.;
  }
  
  // The matrix e(R,R,next) should be orthonormal
  // Renormalize and orthogonalize the axes of inertia
  for( int n=0; n<3; n++ )
  {
    // make column "n" orthogonal to all previous columns "m"
    for( int m=0; m<n; m++ )
    {
      real dot=e(0,m,next)*e(0,n,next)+e(1,m,next)*e(1,n,next)+e(2,m,next)*e(2,n,next);
      e(R,n,next)-=dot*e(R,m,next);
    }
    // normalized column n
    real normInverse=1./max(REAL_MIN,SQRT( SQR(e(0,n,next))+SQR(e(1,n,next))+SQR(e(2,n,next))));
    e(R,n,next)*=normInverse;
      
  }
  

//   for( n=0; n<3; n++ )
//     rotation(R,n)=e(R,0,next)*e0(0,n)+e(R,1,next)*e0(1,n)+e(R,2,next)*e0(2,n);

  applyConstraints( next,applyConstraintsToPosition );


  if( false )
  {
    printF("RigidBodyMotion:takeStepLeapFrog: t=%e, current=%i x(next)=(%6.2e,%6.2e,%6.2e), w(current)=(%6.2e,%6.2e,%6.2e)\n",
	   t,current,x(0,next),x(1,next),x(2,next),w(0,current),w(1,current),w(2,current));
  }
  if( debug & 1 )
  {
    printF("RBM:takeStepLeapFrog: integrate: t0=%e, t=%e, current=%i x(next)=(%6.2e,%6.2e,%6.2e)\n",
	   t0,t,current,x(0,next),x(1,next),x(2,next));
  }
  
  
  return 0;
}


// ============================================================================================
/// \brief Take a time step with the trapezoidal rule (corrector). This is a protected routine.
/// \param t0 (input) : current time (at the start of the step)
/// \param dt : time step.
// ============================================================================================
int RigidBodyMotion::
takeStepTrapezoid( const real t0, const real dt )
{
  assert( current>=0 );
  const int next     = (current+1) % maximumNumberToSave;

  assert( current!=next );

  const real t=t0+dt;

  RealArray A(R,R), Ai(R,R), Lambda(R,R), LambdaInverse(R,R), Omega(R,R), ea(R,R);
  Lambda=0.;          // diagonal matrix that holds moment of inertial eigenvalues
  LambdaInverse=0.;   // Inverse of Lambda
  for( int j=0; j<3; j++ )
  {
    Lambda(j,j)=mI(j);
    LambdaInverse(j,j)=1./mI(j);
  }

  RealArray fv(R), gv(R), fvn(R), gvn(R);
  if( twilightZone )
  { 
    ea = e(R,R,current);
    A  = mult(ea,mult(Lambda,trans(ea)));
    getForceInternal( t0,fv,gv,&A );

    ea = e(R,R,next);
    A  = mult(ea,mult(Lambda,trans(ea)));
    getForceInternal( t ,fvn,gvn,&A );
  }
  else
  {
    fv=f(R,current); gv=g(R,current);
    fvn=f(R,next);   gvn=g(R,next);
  }

  v(R,next)=v(R,current) + .5*dt*( fv/mass + fvn/mass );

  x(R,next)=x(R,current) + .5*dt*( v(R,next)+v(R,current) );
  
  RealArray wDot(R), wDotCurrent(R), eDot(R,R), eDotCurrent(R,R);
  if( true )
  {
    // new way
    //  h' = g, h =A*w,  A= E*Lambda*E^T, A^{-1} = E*Lambda^{-1}*E^T
    //  A * wDot = - Omega*A*w  + g 

    // -- first re-compute wDot for the current level
    ea = e(R,R,current);
    A  = mult(ea,mult(Lambda,trans(ea)));
    Ai = mult(ea,mult(LambdaInverse,trans(ea)));  // A^{-1}
    Omega = getCrossProductMatrix( w(R,current) );

    wDotCurrent = mult( Ai, evaluate(- mult(Omega,mult(A,w(R,current))) + gv) );

    // -- now compute wDot of for the next level
    ea = e(R,R,next);
    A  = mult(ea,mult(Lambda,trans(ea)));
    Ai = mult(ea,mult(LambdaInverse,trans(ea)));  // A^{-1}
    Omega = getCrossProductMatrix( w(R,next) );

    wDot = mult( Ai, evaluate(- mult(Omega,mult(A,w(R,next))) + gvn) );

  }
  else
  {
    // *old way*

    for( int axis=0; axis<3; axis++ )
    {
      const int axisp1=(axis+1) % 3;
      const int axisp2=(axis+2) % 3;
      wDot(axis)=( (mI(axisp1)-mI(axisp2))*w(axisp1,next)*w(axisp2,next) +
		   gvn(0)*e(0,axis,next)+
		   gvn(1)*e(1,axis,next)+
		   gvn(2)*e(2,axis,next) )/mI(axis);
    
      wDotCurrent(axis)=( (mI(axisp1)-mI(axisp2))*w(axisp1,current)*w(axisp2,current)+
			  gv(0)*e(0,axis,current)+
			  gv(1)*e(1,axis,current)+ 
			  gv(2)*e(2,axis,current) )/mI(axis);
    }
  }
  
  w(R,next)=w(R,current)+.5*dt*( wDot(R)+wDotCurrent(R) );

  // eDot =  w X e
  eDot(0,R)=w(1,next)*e(2,R,next)-w(2,next)*e(1,R,next);
  eDot(1,R)=w(2,next)*e(0,R,next)-w(0,next)*e(2,R,next);
  eDot(2,R)=w(0,next)*e(1,R,next)-w(1,next)*e(0,R,next);
  
  eDotCurrent(0,R)=w(1,current)*e(2,R,current)-w(2,current)*e(1,R,current);
  eDotCurrent(1,R)=w(2,current)*e(0,R,current)-w(0,current)*e(2,R,current);
  eDotCurrent(2,R)=w(0,current)*e(1,R,current)-w(1,current)*e(0,R,current);
  

  e(R,R,next)=e(R,R,current)+.5*dt*(eDot(R,R)+eDotCurrent(R,R));

  
  if( numberOfDimensions==2 )
  {
    // 2D: first 2 axes should remain in the plane.
    e(2,R,next)=0.;
    e(R,2,next)=0.;
    e(2,2,next)=1.;
  }
  
  // The matrix e(R,R,next) should be orthonormal
  // Renormalize and orthogonalize the axes of inertia
  for( int n=0; n<3; n++ )
  {
    // make column "n" orthogonal to all previous columns "m"
    for( int m=0; m<n; m++ )
    {
      real dot=e(0,m,next)*e(0,n,next)+e(1,m,next)*e(1,n,next)+e(2,m,next)*e(2,n,next);
      e(R,n,next)-=dot*e(R,m,next);
    }
    // normalized column n
    real normInverse=1./max(REAL_MIN,SQRT( SQR(e(0,n,next))+SQR(e(1,n,next))+SQR(e(2,n,next))));
    e(R,n,next)*=normInverse;
      
  }

  applyConstraints( next,applyConstraintsToPosition );


  return 0;
}



// ============================================================================================
/// \brief Unpack the state vector yv into the arrays xv, vv, omegav, and matrix ea.
// ===========================================================================================
int 
arrayToState( const RealArray & yv, 
              RealArray & xv, RealArray & vv, RealArray & omegav, RealArray & ea )
{
  Range R3=3;
  int m=0;
  xv(R3)=yv(R3+m); m+=3;
  vv(R3)=yv(R3+m); m+=3;
  omegav(R3)=yv(R3+m); m+=3;
  for( int j=0; j<3; j++ )
  {
    ea(R3,j)=yv(R3+m); m+=3;
  }
  return 0;
}

// ============================================================================================
/// \brief Pack the arrays xv, vv, omegav, and matrix ea into the state vector yv.
// ===========================================================================================
int 
stateToArray( const RealArray & xv, const RealArray & vv, const RealArray & omegav, const RealArray & ea,
              RealArray & yv )
{
  Range R3=3;
  int m=0;
  yv(R3+m)=xv(R3); m+=3;
  yv(R3+m)=vv(R3); m+=3;
  yv(R3+m)=omegav(R3); m+=3;
  for( int j=0; j<3; j++ )
  {
    yv(R3+m)=ea(R3,j); m+=3;
  }

  return 0;
}

// ============================================================================================
/// \brief Take a time step with the implicit Runge-Kutta method. This is a protected routine.
/// \param t0 (input) : current time (at the start of the step)
/// \param dt : time step.
// ============================================================================================
int RigidBodyMotion::
takeStepImplicitRungeKutta( const real t0, const real dt )
{
  
  assert( current>=0 );
  const int next     = (current+1) % maximumNumberToSave;

  const int orderOfAccuracy = dbase.get<int>("orderOfAccuracy");
  
  const int numberOfStateVariables = 3 + 3 + 3 + 9;  // x, v, omega, E
  RealArray yv(numberOfStateVariables), yv0(numberOfStateVariables),yvn(numberOfStateVariables);
  RealArray kv1(numberOfStateVariables);
  
  // pack yv with current state
  // stateToArray( x(R,current), v(R,current), w(R,current), e(R,R,current), yv); // does this work -- bases?
  int m=0;
  yv(R  )=x(R,current); m+=3;
  yv(R+m)=v(R,current); m+=3;
  yv(R+m)=w(R,current); m+=3;
  for( int j=0; j<3; j++ )
  {
    yv(R+m)=e(R,j,current); m+=3;
  }

  if( orderOfAccuracy==1 )
  {
    // ---- DIRK order 1 : Back Euler: -----
    //     y' = f(y,t)
    //     k1 = f(y(n)+dt*k1,t+dt) : implicit equation for k1
    //     y(n+1) = y(n) + dt*k1
    //
    // To solve k1 = f(y(n)+dt*k1,t+dt), let y=y(n)+dt*k1, then
    //      y + dt*f(y,t+dt) = y(n) , k1=(y-yn)/dt 
    //
    //
    real aii=1., ci=1., b1=1.; // Backward Euler
    yv0=yv;  // yv0=yn + dt*sum_j=0^{i-1} k_j
    // NOTE: the yv argument is the initial guess -- we could do better here
    dirkImplicitSolve( dt,aii,t0+ci*dt,yv,yv0, kv1); 
    
    yvn = yv + dt*b1*( kv1 );

  }
  else if( orderOfAccuracy==2 )
  {
    //  ---- DIRK order 2 : Implicit mid-point --

    real aii=.5, ci=.5, b1=1.; // Implicit mid-point

    yv0=yv;  //  yv0=yn + dt*sum_j=0^{i-1} k_j
    dirkImplicitSolve( dt,aii,t0+ci*dt,yv,yv0, kv1);  // yv=initial guess

    yvn = yv + dt*b1*( kv1 );

  }
  else if( orderOfAccuracy==3 )
  {
    // ---- DIRK two-stage order 3, A-stable 
    //   Formula from Crouzeix, cf. Alexander 1977 SIAM J. Anal. 14, no 6. 

    RealArray kv2(numberOfStateVariables);
    
    real sqi3 = 1./sqrt(3.);

    real a11=.5*(1.+sqi3),                    c1=.5*(1.+sqi3), b1=.5; 
    real a21=-sqi3,        a22=.5*(1.+sqi3),  c2=.5*(1.-sqi3), b2=.5; 

    yv0=yv;  // yv0=yn + dt*sum_j=0^{i-1} k_j
    dirkImplicitSolve( dt,a11,t0+c1*dt,yv,yv0, kv1);  // yv=initial guess

    yv0=yv + dt*( a21*kv1 );
    dirkImplicitSolve( dt,a22,t0+c2*dt,yv,yv0, kv2);  // yv=initial guess  *FIX ME*

    yvn = yv + dt*( b1*kv1 + b2*kv2 );

  }
  else if( orderOfAccuracy==4 )
  {  

    // ---- DIRK 4-stage order 4, A0-stable 
    //   Formula from Jackson and Norsett (1990)   -- See: for A and L stable methods? 
    // Iserles, A. and Nørsett, S. P. (1990). On the Theory of Parallel Runge-Kutta Methods. IMA
    //   Journal of Numerical Analysis 10: 463-488.
 
    RealArray kv2(numberOfStateVariables), kv3(numberOfStateVariables), kv4(numberOfStateVariables);

    real a11=1.,                                        c1=1.,     b1=11./72.; 
    real a21=0.,       a22=3./5.,                       c2=3./5.,  b2=25./72.;
    real a31=171./44., a32=-215/44., a33=1.,            c3=0.,     b3=11./72.;
    real a41=-43./20., a42=39./20.,  a43=0., a44=3./5., c4=2./5.,  b4=25./72.;

    yv0=yv;  // yv0=yn + dt*sum_j=0^{i-1} k_j
    dirkImplicitSolve( dt,a11,t0+c1*dt,yv,yv0, kv1);  // yv=initial guess

    yv0=yv + dt*( a21*kv1 );
    dirkImplicitSolve( dt,a22,t0+c2*dt,yv,yv0, kv2);  // yv=initial guess  *FIX ME*

    yv0=yv + dt*( a31*kv1 + a32*kv2 );
    dirkImplicitSolve( dt,a33,t0+c3*dt,yv,yv0, kv3);  // yv=initial guess  *FIX ME*

    yv0=yv + dt*( a41*kv1 + a42*kv2 + a43*kv3 );
    dirkImplicitSolve( dt,a44,t0+c4*dt,yv,yv0, kv4);  // yv=initial guess  *FIX ME*

    yvn = yv + dt*( b1*kv1 + b2*kv2 + b3*kv3 + b4*kv4 );


  }
  else
  {
    OV_ABORT("finish me");
  }
  
  // Unpack yvn into next state
  // arrayToState( yvn, x(R,next), v(R,next), w(R,next), e(R,R,next)  );
  m=0;
  x(R,next)=yvn(R+m); m+=3;
  v(R,next)=yvn(R+m); m+=3;
  w(R,next)=yvn(R+m); m+=3;
  for( int j=0; j<3; j++ )
  {
    e(R,j,next)=yvn(R+m); m+=3;
  }

  // The matrix e(R,R,next) should be orthonormal
  // Renormalize and orthogonalize the axes of inertia

  if( numberOfDimensions==2 )
  {
    // 2D: first 2 axes should remain in the plane.
    e(2,R,next)=0.;
    e(R,2,next)=0.;
    e(2,2,next)=1.;
  }
  
  for( int n=0; n<3; n++ )
  {
    // make column "n" orthogonal to all previous columns "m"
    for( int m=0; m<n; m++ )
    {
      real dot=e(0,m,next)*e(0,n,next)+e(1,m,next)*e(1,n,next)+e(2,m,next)*e(2,n,next);
      e(R,n,next)-=dot*e(R,m,next);
    }
    // normalized column n
    real normInverse=1./max(REAL_MIN,SQRT( SQR(e(0,n,next))+SQR(e(1,n,next))+SQR(e(2,n,next))));
    e(R,n,next)*=normInverse;
      
  }

  applyConstraints( next,applyConstraintsToPosition );

  
  return 0;
}



// ============================================================================================
/// \brief Take a (predictor) time step by extrapolation in time. This can be used with the
///  DIRK schemes, for example.
/// \param t0 : current time.
/// \param dt : time step.
// ============================================================================================
int RigidBodyMotion::
takeStepExtrapolation( const real t0, const real dt )
{
  // const bool includeAddedMass = dbase.get<bool>("includeAddedMass");
  
  const real t = t0+dt;

  const int previous = (current-1+maximumNumberToSave) % maximumNumberToSave;
  const int next     = (current+1) % maximumNumberToSave;

  const int orderOfAccuracy = dbase.get<int>("orderOfAccuracy");
  int orderOfExtrapPredict = dbase.get<int>("orderOfExtrapolationPredictor");
  if( orderOfExtrapPredict<0 )
    orderOfExtrapPredict=orderOfAccuracy;


  if( numberSaved==1 )
  {
    // constant extrapolation
    x(R,next)= x(R,current);
    v(R,next)= v(R,current);
    w(R,next)= w(R,current);
    e(R,R,next)= e(R,R,current);
  }
  else if( numberSaved<=2 )
  {
    // Linear extrapolation from the current and previous times.
    real dtOld= numberSaved>1 ? time(current)-time(previous) : 0.;
    real dtRatio;
    if( dtOld!=0. )
      dtRatio=dt/dtOld;
    else
      dtRatio=0.;
    real ab1= 1.+dtRatio; // normally 2
    real ab2= -dtRatio;   // normally -1

    x(R,next)  =ab1*x(R,current)  +ab2*x(R,previous);
    v(R,next)  =ab1*v(R,current)  +ab2*v(R,previous);
    w(R,next)  =ab1*w(R,current)  +ab2*w(R,previous);
    e(R,R,next)=ab1*e(R,R,current)+ab2*e(R,R,previous);
  }
  else
  {
    // Lagrange interpolation in time -- should we do higher-order here too?

    const int prev2 = (current-2+maximumNumberToSave) % maximumNumberToSave;
    real t1=time(current), t2=time(previous), t3=time(prev2);
    real c1 = ( (t-t2)*(t-t3) )/( (t1-t2)*(t1-t3) );
    real c2 = ( (t-t3)*(t-t1) )/( (t2-t3)*(t2-t1) );
    real c3 = ( (t-t1)*(t-t2) )/( (t3-t1)*(t3-t2) );

    x(R,next)  = c1*x(R,current)  + c2*x(R,previous)  + c3*x(R,prev2);
    v(R,next)  = c1*v(R,current)  + c2*v(R,previous)  + c3*v(R,prev2);
    w(R,next)  = c1*w(R,current)  + c2*w(R,previous)  + c3*w(R,prev2);
    e(R,R,next)= c1*e(R,R,current)+ c2*e(R,R,previous)+ c3*e(R,R,prev2);

    if( logFile!=NULL )
      fPrintF(logFile," EXTRAP: (t1,t2,t3)=(%9.3e,%9.3e,%9.3e) c1=%8.2e, c2=%8.2e c3=%8.2e\n",t1,t2,t3,c1,c2,c3);
      

  }
    
  if( logFile!=NULL )
  {
    fPrintF(logFile," EXTRAP to t=%9.3e: x(next)=(%11.4e,%11.4e,%11.4e) v(next)=(%11.4e,%11.4e,%11.4e) "
	    "w(next)=(%11.4e,%11.4e,%11.4e)\n",t,x(0,next),x(1,next),x(2,next), 
	    v(0,next),v(1,next),v(2,next),w(0,next),w(1,next),w(2,next));
  }


//   if( numberSaved==1 )
//   {
//     // constant extrapolation
//     x(R,next)= x(R,current);
//     v(R,next)= v(R,current);
//     w(R,next)= w(R,current);
//     e(R,R,next)= e(R,R,current);
//   }
//   else if( numberSaved==2 || orderOfExtrapPredict<=2 )
//   {
//     // linear extrapolation
//     real t1=time(current), t2=time(prev);
//     real c1 = (t-t2)/(t1-t2);
//     real c2 = (t-t1)/(t2-t1);

//     x(R,next)= c1*x(R,current) + c2*x(R,previous);
//     v(R,next)= c1*v(R,current) + c2*v(R,previous);
//     w(R,next)= c1*w(R,current) + c2*w(R,previous);
//     e(R,R,next)= c1*e(R,R,current) + c2*e(R,R,previous);
//   }
//   else
//   {
//     // Quadratic extrapolation
//     const int prev2 = (current-2+maximumNumberToSave) % maximumNumberToSave;
//     real t1=time(current), t2=time(prev), t3=time(prev2);
//     real c1 = ( (t-t2)*(t-t3) )/( (t1-t2)*(t1-t3) );
//     real c2 = ( (t-t3)*(t-t1) )/( (t2-t3)*(t2-t1) );
//     real c3 = ( (t-t1)*(t-t2) )/( (t3-t1)*(t3-t2) );
      
//     x(R,next)= c1*x(R,current) + c2*x(R,previous) + c3*x(R,prev2);
//     v(R,next)= c1*v(R,current) + c2*v(R,previous) + c3*v(R,prev2);
//     w(R,next)= c1*w(R,current) + c2*w(R,previous) + c3*w(R,prev2);
//     e(R,R,next)= c1*e(R,R,current) + c2*e(R,R,previous) + c3*e(R,R,prev);
//   }

//   if( logFile!=NULL )
//   {
//     fPrintF(logFile," predict (extrap): t=%9.3e, x(next)=(%11.4e,%11.4e,%11.4e), v(next)=(%11.4e,%11.4e,%11.4e), cur=%i, next=%i\n"
// 	    "     w(next)=(%11.4e,%11.4e,%11.4e) [ orderOfExtrapolationPredictor=%i].\n",
// 	    t,x(0,next),x(1,next),x(2,next),v(0,next),v(1,next),v(2,next),current,next,
//             w(0,next),w(1,next),w(2,next),orderOfExtrapPredict);
//     fflush(logFile);
//   }
  
  return 0;
}


// =======================================================================================================
/// \brief: Return the force and torque at time t (interpolate in time if necessary). This is a
/// protected routine. The force and torque do NOT include the added mass terms.
/// \param t (input) : evaluate the force and torque at this time.
/// \param fv, gv (output) : force and torque
/// \param pA (input) : for twilightzone forcing supply A (the angular momentum matrix, h=A*w)
// =======================================================================================================
int RigidBodyMotion::
getForceInternal(const real t, RealArray & fv, RealArray & gv, RealArray *pA /* =NULL */ ) const
{
  
  if( !twilightZone )
  {
    assert( current>=0 );
    const int previous = (current-1+maximumNumberToSave) % maximumNumberToSave;
    const int next = (current+1) % maximumNumberToSave;

    const int orderOfAccuracy = dbase.get<int>("orderOfAccuracy");

    assert( numberSaved>=1 );

    // The user may have supplied values of the forcings at negative times.
    const int numberOfForcingsSaved = numberSaved + dbase.get<int>("numberOfPastTimeValues");

    // Linear interpolation in time:
    if( orderOfAccuracy<=2 || numberOfForcingsSaved<=1 ) // Note: numberSaved does not include solution "next"
    {
      // Linear interpolation in time:
      real t1=time(next), t2=time(current);
      real c1 = (t-t2)/(t1-t2);
      real c2 = (t-t1)/(t2-t1);
      fv= c1*f(R,next) + c2*f(R,current);
      gv= c1*g(R,next) + c2*g(R,current);
      
    }
    else if( orderOfAccuracy==3 || numberOfForcingsSaved<=2 )
    {
      // Quadratic Lagrange interpolation in time
      real t1=time(next), t2=time(current), t3=time(previous);
      real c1 = ( (t-t2)*(t-t3) )/( (t1-t2)*(t1-t3) );
      real c2 = ( (t-t3)*(t-t1) )/( (t2-t3)*(t2-t1) );
      real c3 = ( (t-t1)*(t-t2) )/( (t3-t1)*(t3-t2) );
      

      fv= c1*f(R,next) + c2*f(R,current)+ c3*f(R,previous);
      gv= c1*g(R,next) + c2*g(R,current)+ c3*g(R,previous);

    }

    else if( orderOfAccuracy==4 )
    {
      // Cubic Lagrange interpolation in time, use 4 time levels
      const int prev2 = (current-2+maximumNumberToSave) % maximumNumberToSave;

      real t1=time(next), t2=time(current), t3=time(previous), t4=time(prev2);
      real c1 = ( (t-t2)*(t-t3)*(t-t4) )/( (t1-t2)*(t1-t3)*(t1-t4) );
      real c2 = ( (t-t3)*(t-t4)*(t-t1) )/( (t2-t3)*(t2-t4)*(t2-t1) );
      real c3 = ( (t-t4)*(t-t1)*(t-t2) )/( (t3-t4)*(t3-t1)*(t3-t2) );
      real c4 = ( (t-t1)*(t-t2)*(t-t3) )/( (t4-t1)*(t4-t2)*(t4-t3) );
      
      fv= c1*f(R,next) + c2*f(R,current)+ c3*f(R,previous)+ c4*f(R,prev2);
      gv= c1*g(R,next) + c2*g(R,current)+ c3*g(R,previous)+ c4*g(R,prev2);

    }
    else
    {
      printF("RigidBodyMotion::getForceInternal:ERROR: interpolation for orderOfAccuracy=%i not implemented\n",orderOfAccuracy);
      OV_ABORT("ERROR");
    }
    
    
  }
  else 
  {
    // --- Evaluate the forcing for twilightzone ---

    // -- evaluate the exact solution and it's time derivative
    RealArray xExact(R), vExact(R), wExact(R);
    RealArray xDotExact(R), vDotExact(R), wDotExact(R);
    getExactSolution( 0, t, xExact, vExact, wExact  ); 
    getExactSolution( 1, t, xDotExact, vDotExact, wDotExact  ); 

    fv = mass*vDotExact;

    // RealArray Lambda(R,R), ea(R,R), A(R,R), OmegaExact(R,R);
    // A = mult(ea,mult(Lambda,trans(ea)));
    RealArray OmegaExact(R,R);
    OmegaExact = getCrossProductMatrix( wExact );

    // A' = Omega*A - A*Omega  
    assert( pA!=NULL );
    RealArray & A = *pA;
    gv = mult(A,wDotExact) + mult(OmegaExact,mult(A,wExact)); 

    const bool includeAddedMass = dbase.get<bool>("includeAddedMass");
    if( includeAddedMass )
    {
      RealArray A11(R,R), A12(R,R), A21(R,R), A22(R,R);
      getAddedMassMatrices( t, A11 , A12 , A21, A22 );

      fv += mult(A11,vExact) + mult(A12,wExact);
      gv += mult(A21,vExact) + mult(A22,wExact);
    }
  
  }
  
  return 0;

}


// =======================================================================================================
/// \brief: Solve the implicit DIRK equation for kv. This is a protected routine.
/// \details This code is based on the matlab code in ??
// 
// --- Solve:
//      M*(yv-yv0) - aii*dt*f(yv,tc) = 0 
//
//   kv = (yv-yv0)/(aii*dt) 
// Input:
//   aii, tc : diagonal weight and time
//   yv = initial guess 
//   yv0 
// =======================================================================================================
int RigidBodyMotion::
dirkImplicitSolve( const real dt, const real aii, const real tc, const RealArray & yv, const RealArray &yv0, 
                   RealArray & kv )
{

  const int orderOfAccuracy = dbase.get<int>("orderOfAccuracy");
  
  // convergence tol. for Newton iteration:
  // real rkTol=1.e-5;  // Note: if converging quadratically then next correction will be rkTol^2   **FIX ME**
  // real rkTol=1.e-9;  // Note: if converging quadratically then next correction will be rkTol^2   **FIX ME**

  const real rkTol=dbase.get<real>("toleranceNewton");

  RealArray xvn(R), vvn(R), omegavn(R), ean(R,R);
  arrayToState( yv, xvn, vvn, omegavn, ean );
  
  RealArray xv0(R), vv0(R), omegav0(R), ea0(R,R);
  arrayToState( yv0, xv0, vv0, omegav0, ea0 );
  
  // Initial guess:
  RealArray xvk(R), vvk(R), omegavk(R), eak(R,R);
  vvk = vvn;  omegavk = omegavn; eak = ean;

  RealArray A11(R,R), A12(R,R), A21(R,R), A22(R,R);
  getAddedMassMatrices( tc, A11 , A12 , A21, A22 );

  real adt = aii*dt; 

  const int m = 3 + 3 + 9;    // number of unknowns in implicit system [ v, omega, ea ]
  Range Rn = m;
  RealArray rk(Rn);     // holds residual 
  RealArray Jk(Rn,Rn);  // Jacobian
  RealArray dy(Rn);     // correction
  RealArray Ak(R,R), Lambda(R,R), Omegak(R,R);
  RealArray eye(R,R), hk(R), hkStar(R,R), dOmega1(R), eaStar(R,R);

  Jk=0.;
  Lambda=0.; // diagonal matrix that holds moment of inertial eigenvalues
  eye=0.;    // 3x3 indentity matrix
  for( int j=0; j<3; j++ )
  {
    Lambda(j,j)=mI(j);
    eye(j,j)=1.;
  }
  
  // Forcing and torque:
  RealArray fvnp1(R), gvnp1(R);

//   // Do this for now: linear interpolation in time:
//   assert( current>=0 );
//   const int next     = (current+1) % maximumNumberToSave;
//   const real t0 = time(current);
//   real alpha = (tc-t0)/dt;
//   fvnp1=(1.-alpha)*f(R,current)+alpha*f(R,next);
//   gvnp1=(1.-alpha)*g(R,current)+alpha*g(R,next);

  RealArray OmegaExact, omegavDotExact, omegavExact;
  if( twilightZone )
  { // The angular momentum matrix A is needed for twilightZone forcing:
    Ak = mult(eak,mult(Lambda,trans(eak)));

    // Twilightzone also needs omegavExact, OmegaExact, and omegavDotExact.
    OmegaExact.redim(R,R); omegavDotExact.redim(R); omegavExact.redim(R);
    RealArray xExact(R), vExact(R);
    RealArray xDotExact(R), vDotExact(R);
    getExactSolution( 0, tc, xExact, vExact, omegavExact  ); 
    getExactSolution( 1, tc, xDotExact, vDotExact, omegavDotExact  ); 

    OmegaExact = getCrossProductMatrix( omegavExact );     
  }
  getForceInternal(tc, fvnp1, gvnp1, &Ak);

  // ::display(Lambda,"Lambda","%5.1f ");
  // ::display(eye,"eye","%5.1f ");

  int maxIterations=20;
  real maxCorrection=REAL_MAX;
  int k=0;
  for( k=0; k<maxIterations; k++ )
  {
    // Ak = eak*Lambda*eak'; % inertia matrix 
    Ak = mult(eak,mult(Lambda,trans(eak)));
    // Omegak = [ 0. -omegavk(3) omegavk(2); omegavk(3) 0. -omegavk(1); -omegavk(2) omegavk(1) 0. ];
    Omegak = getCrossProductMatrix( omegavk );  //  matrix for [ omegavk X ]

    // ::display(Ak,"Ak","%5.1f ");
    // ::display(omegavk,"omegavk","%8.2e ");
    // ::display(Omegak,"Omegak","%8.2e ");
    

    if( twilightZone )
    {
      // With twilightzone the torque depends on the computed value of A so we need to re-evaluate g
      getForceInternal(tc, fvnp1, gvnp1, &Ak);
    }
    
    // residual
    //  v: 
    rk(R) = mass*(vvk-vv0) - adt*( -mult(A11,vvk) - mult(A12,omegavk) + fvnp1 );


    // omega: (LEAK found *wdh* 2012/02/19 -- need evaluate
    rk(R+3) = mult(Ak,evaluate(omegavk-omegav0)) - adt*( -mult(Omegak,mult(Ak,omegavk)) 
              -mult(A21,vvk) - mult(A22,omegavk) + gvnp1 );

    // E:   
    int mr=6;
    for( int j=0; j<3; j++ )
    {
      rk(R+mr) = eak(R,j)-ea0(R,j) - adt*( mult(Omegak,eak(R,j)) );
      mr=mr+3; 
    }
    
    // ::display(mult(Ak,omegavk),"Ak*omegavk","%8.2e ");

    if( debug & 2 )
    {
      printF("DIRK: k=%d, max(|resid|)=%8.2e\n",k,max(fabs(rk))); 
      // ::display(rk,"residual","%8.2e ");
    }

    
    // Fill the Jacobian
    // v: 
    Jk(R,R  ) = mass*eye + adt*A11;   // d(fv)/d(v)
    Jk(R,R+3) = adt*A12;              // d(fv)/d(omega)

    //  omega:
    hk = mult(Ak,omegavk);
    hkStar = getCrossProductMatrix( hk );  // matrix for [ hk X ]
    Jk(R+3,R  ) = adt*A21;               // d(fomega)/d(v)
    Jk(R+3,R+3) = Ak + adt*(A22+mult(Omegak,Ak)-hkStar);    // d(fomega)/d(omega)
    
    dOmega1 = omegavk-omegav0;
    if( twilightZone )
    {
      // add on the correction for gvnp1 depending on A and thus on E. 
      dOmega1 = dOmega1 - adt*omegavDotExact;
    }

    mr=6;
    for( int j=0; j<3; j++ )
    {
      // Here is d(fomega)/d(ev(j))
      Jk(R+3,R+mr) = Lambda(j,j)*( dot(eak(R,j),dOmega1)*eye + mult(eak(R,j),trans(dOmega1)) ) 
	+ (Lambda(j,j)*adt)*mult(Omegak, evaluate(dot(eak(R,j),omegavk)*eye + mult(eak(R,j),trans(omegavk))) );

      if( twilightZone )
      {
        // add on the correction for gvnp1 depending on A and thus on E. 
	Jk(R+3,R+mr) +=
	  (Lambda(j,j)*adt)*mult(OmegaExact, evaluate(dot(eak(R,j),omegavExact)*eye + mult(eak(R,j),trans(omegavExact))) );
      }
      
      mr=mr+3; 
    }
				  
    // printF("dot(eak(R,0),dOmega1)=%8.2e\n",dot(eak(R,0),dOmega1));
    // printF("dot(eak(R,1),dOmega1)=%8.2e\n",dot(eak(R,1),dOmega1));

    // ::display(dOmega1,"dOmega1");
    // ::display(trans(dOmega1),"trans(dOmega1)");
    


    mr=6;
    for( int j=0; j<3; j++ )
    {
      eaStar = getCrossProductMatrix( eak(R,j) );  //  matrix for [ eak(1:3,j) X ]
      Jk(R+mr,R+3   ) = adt*eaStar;                // d(fea)/d(omega)
      Jk(R+mr,R+mr  ) = eye - adt*Omegak;          // d(fea(i))/d(ea(i))
      mr=mr+3; 
    }

    // ::display(Jk,"Jacobian");

    // Solve:
    dy = solve(Jk,rk);
    
    if( debug & 4 )
    {
      RealArray r(Rn);
      r=rk-mult(Jk,dy);
      printF("Max. error in solving Jk*dy=rk is %8.2e\n",max(fabs(r)));
    }
    // OV_ABORT("done");
    

    maxCorrection = max(abs(dy));
    if( debug & 2 )
    {
      printF("RBM:DIRK: k=%d, correction: max(|dy|)=%8.2e (tc=%9.3e)\n",k,maxCorrection,tc); 
    }

    // update the current solution
    vvk = vvk - dy(R);  
    omegavk = omegavk - dy(R+3);
    mr=6;
    for( int j=0; j<3; j++ )
    {
      // correction should be orthogonal to ean *but* this messes up Newton convergence
      // dy(mr:mr+2) = dy(mr:mr+2) - (dy(mr:mr+2)'*ean(1:3,j))*ean(1:3,j);
      eak(R,j) = eak(R,j) - dy(R+mr);
      mr=mr+3; 
    }

    //  fprintf('DIRK: eak*eak^T - I : k=%d\n',k);
    //  eak*eak' - eye(3,3)
    //  pause;

    if( maxCorrection<rkTol ) 
      break;

  }
  
  if( maxCorrection>rkTol )
  {
    printF("RBM:dirkImplicitSolve:WARNING: No convergence in Newton after %d iterations,. maxCorrection=%8.2e, tol=%8.2e\n",
	   maxIterations,maxCorrection,rkTol);
  }
  else
  {
    
    if( debug & 1 )
    {
      printF("RBM:DIRK%i: %i Newton iterations required.\n",orderOfAccuracy,k+1);
    }
  }

  if( logFile!=NULL )
  {
    rk(R) = -mult(A11,vvk) - mult(A12,omegavk) + fvnp1; 
    rk(R+3) = -mult(A21,vvk) - mult(A22,omegavk) + gvnp1;
    fPrintF(logFile,
            "RBM:DIRK%i: t=%9.3e: its=%i, v =(%12.5e,%12.5e,%12.5e) w =(%12.5e,%12.5e,%12.5e)\n"
	    "                               v0=(%12.5e,%12.5e,%12.5e) w0=(%12.5e,%12.5e,%12.5e)\n"
	    "  -A11*v-A12*w+f = (%12.5e,%12.5e,%12.5e) f=(%12.5e,%12.5e,%12.5e) \n"
	    "  -A21*v-A22*w+g = (%12.5e,%12.5e,%12.5e) g=(%12.5e,%12.5e,%12.5e) \n",
            orderOfAccuracy,tc,k+1,
            vvk(0),vvk(1),vvk(2), omegavk(0),omegavk(1),omegavk(2),
	    vv0(0),vv0(1),vv0(2), omegav0(0),omegav0(1),omegav0(2),
            rk(0),rk(1),rk(2), fvnp1(0), fvnp1(1), fvnp1(2), rk(3),rk(4),rk(5), gvnp1(0),gvnp1(1),gvnp1(2));
  }
  
  // ::display(vvk,"RBM:DIRK: x' = v");
  // ::display((vvk-vv0)/adt,"RBM:DIRK: v'");
  


  // Fill in the result into kv
  
  kv(R  )=vvk;                         // x' = v   => k(v) = v
  kv(R+3)=(vvk-vv0)/adt;               // v'
  kv(R+6)=(omegavk-omegav0)/adt;       // w'
  int mr=9;
  for( int j=0; j<3; j++ )
  {
    kv(R+mr) = (eak(R,j)-ea0(R,j))/adt;  // e'
    mr=mr+3; 
  }
  

  return 0;
}

// ============================================================================================
/// \brief Turn on (or off) twilight-zone forcing.
/// \param trueOrFalse (input) : turn on or off 
/// \param type (input) : twilight-zone type
// ============================================================================================
int RigidBodyMotion::
setTwilightZone( bool trueOrFalse, TwilightZoneTypeEnum type /*= trigonometricTwilightZone */   )
{
  twilightZone=trueOrFalse;
  twilightZoneType=type;

  if( !dbase.has_key("amp") ) dbase.put<RealArray>("amp");
  if( !dbase.has_key("freq") ) dbase.put<RealArray>("freq");
  if( !dbase.has_key("tOffset") ) dbase.put<RealArray>("tOffset");

  RealArray & amp = dbase.get<RealArray>("amp");
  RealArray & freq = dbase.get<RealArray>("freq");
  RealArray & tOffset = dbase.get<RealArray>("tOffset");
    
  // Trigonometric:
  //     vv = amp(1,1)*cos( freq(1,1)*( t-tOffset(1,1) )

  amp.redim(R,2); freq.redim(R,2); tOffset.redim(R,2);

  // v 
  amp(0,0)=1.;  freq(0,0)=Pi*2.5;  tOffset(0,0)=0.;
  amp(1,0)=1.5; freq(1,0)=Pi*1.75; tOffset(1,0)=.25;
  amp(2,0)=.75; freq(2,0)=Pi*1.5;  tOffset(2,0)=.5;
// omega:
  amp(0,1)=.8;  freq(0,1)=Pi*1.5;  tOffset(0,1)=.25;
  amp(1,1)=1.2; freq(1,1)=Pi*2.0;  tOffset(1,1)=.75;
  amp(2,1)=.90; freq(2,1)=Pi*1.2;  tOffset(2,1)=.35; 

  return 0;
}


// ============================================================================================
/// \brief Evaluate the exact solution (for twilightZone)
/// \param deriv (input) : evaluate this derivative of the exact solution
/// \param t (input) : time
/// \param x,v,w (output)
// ============================================================================================
int RigidBodyMotion:: 
getExactSolution( const int deriv, const real t, RealArray & xe, RealArray & ve , RealArray & we ) const
{
  if( twilightZone )
  {
    if( twilightZoneType ==polynomialTwilightZone )
    {
      printF("getExactSolution:ERROR: polynomialTwilightZone -- finish me!\n");
      OV_ABORT("ERROR");
    }
    else if( twilightZoneType==trigonometricTwilightZone )
    {

      const RealArray & amp = dbase.get<RealArray>("amp");
      const RealArray & freq = dbase.get<RealArray>("freq");
      const RealArray & tOffset = dbase.get<RealArray>("tOffset");

      if( deriv==0 )
      {
	for( int j=0; j<3; j++ )
	{
	  xe(j) = amp(j,0)/freq(j,0)*sin( freq(j,0)*( t-tOffset(j,0) ) );
	  ve(j) = amp(j,0)*cos( freq(j,0)*( t-tOffset(j,0) ));
	  we(j) = amp(j,1)*cos( freq(j,1)*( t-tOffset(j,1) ));
	}
	
      }
      else if( deriv==1 )
      {
	for( int j=0; j<3; j++ )
	{
	  xe(j) =  amp(j,0)*cos( freq(j,0)*( t-tOffset(j,0) ) );
	  ve(j) = -amp(j,0)*freq(j,0)*sin( freq(j,0)*( t-tOffset(j,0) ));
	  we(j) = -amp(j,1)*freq(j,1)*sin( freq(j,1)*( t-tOffset(j,1) ));
	}
      }
      else
      {
	printF("getExactSolution:ERROR:trigonometricTwilightZone: deriv=%d\n",deriv);
	OV_ABORT("ERROR");
      }
    }
    else
    {
      printF("getExactSolution:ERROR: unknown option: twilightZone=%d\n",twilightZone);
      OV_ABORT("ERROR");
    }
  }

  return 0;
}



//\begin{>>RigidBodyMotionInclude.tex}{\subsection{integrate}} 
real RigidBodyMotion:: 
getTimeStepEstimate() const
// ==================================================================================
// /Description:
//     Estimate the maximum allowable time step.
//
// /Return value:
//    An estimate of the maximum time step. Return -1. if no estimate is available
//   
//
//\end{RigidBodyMotionInclude.tex}  
// ==================================================================================
{
  if( numberOfSteps<2 )
  {
    printF("RigidBodyMotion::getTimeStepEstimate:ERROR: numberOfSteps<2\n");
    return -1.;
  }

  real dtMax=-1.;

  assert( current >=0 );
  const int previous = (current-1+maximumNumberToSave) % maximumNumberToSave;

  // do this for now -- really need to compute eigenvalues of the Jacobian
  real dx = sqrt(sum(SQR(x(R,current)-x(R,previous))));
  real dv = sqrt(sum(SQR(v(R,current)-v(R,previous))));
  real df = sqrt(sum(SQR(f(R,current)-f(R,previous))));
 

  real fx=0., fv=0.; 
  const real eps = sqrt(REAL_MIN*100.);
   
  if( dx>eps )
    fx = df/dx;
     
  if( dv>eps )
    fv = df/dv;
   

  // estimate lambda*dt where lambda is the maximum modulus of the eigenvalues
  real lambda;
  real des = fv*fv+4.*fx;
  if(  des>0. )
  {
    des=sqrt(des);
    lambda = .5*max( fabs(fv+des), fabs(fv-des) );
  }
  else
  {
    lambda = .5*sqrt(fabs(fx));
  }
   
  const real alpha=1.; // bound on stability region -- choose 1 to be safe (probably=2)
  if( lambda>eps )
    dtMax = (alpha*mass)/lambda; 
  else
    dtMax=-1.;

//    printf("RigidBodyMotion::getTimeStepEstimate: v(current)=(%8.2e,%8.2e,%8.2e) v(prev)=(%8.2e,%8.2e,%8.2e)\n",
//  	 v(0,current),v(1,current),v(2,current), v(0,previous),v(1,previous),v(2,previous));
  
  if( false )
  {
    printF("RigidBodyMotion::getTimeStepEstimate: dx=%8.2e, dv=%8.2e, df=%8.2e, fx=%8.2e, fv=%8.2e, "
           "lambda=%8.2e, dtMax=%8.2e\n", dx,dv,df, fx,fv,lambda,dtMax);
  }
  
  return dtMax;
  
}


  
//\begin{>>RigidBodyMotionInclude.tex}{\subsection{setPosition}} 
int RigidBodyMotion::
getInitialConditions(RealArray & xInitial /* = Overture::nullRealArray() */ , 
                     RealArray & vInitial /* = Overture::nullRealArray() */ , 
                     RealArray & wInitial /* = Overture::nullRealArray() */ ,
                     RealArray & axesOfInertia /* = Overture::nullRealArray() */ ) const
// =======================================================================================
// /Description:
//   Get the current values for the inital conditions.
// 
// /x0(0:2), vInitial(0:2), wInitial(0:2) axesOfInertia(0:2,0:2)  (output) : if any of these arrays is
// dimensioned large enough on input then it is filled in.
//
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  if( xInitial.getLength(0)>=3 )
    xInitial(R)=x0;

  if( vInitial.getLength(0)>=3 ) 
    vInitial(R)=v0;
  
  if( wInitial.getLength(0)>=3 ) 
    wInitial(R)=w0;

  if( numberOfDimensions==3 && axesOfInertia.getLength(0)>2 && axesOfInertia.getLength(1)>2 )
    axesOfInertia(R,R)=e0(R,R);
  
  return 0;
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{getMass}} 
real RigidBodyMotion::
getMass() const
// =======================================================================================
// /Description:
//    Return the mass.
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  return mass;
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{setMass}} 
void RigidBodyMotion::
setMass(const real totalMass ) 
// =======================================================================================
// /Description:
//    Specify the total mass.
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  mass=totalMass;
  if( ((initialConditionsGiven/2)%2)==0 )
    initialConditionsGiven +=2; // this means the mass has been set
}


//\begin{>>RigidBodyMotionInclude.tex}{\subsection{getMomentsOfInertia}} 
RealArray  RigidBodyMotion::
getMomentsOfInertia() const
// =======================================================================================
// /Description:
//    Return the 3 moments of inertia.
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  return mI;
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{getDensity}} 
real RigidBodyMotion::
getDensity() const
// =======================================================================================
// /Description:
//    Return the density, if known. If not known return a negative value.
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  return density;
}



//\begin{>>RigidBodyMotionInclude.tex}{\subsection{getAxesOfInertia}} 
int RigidBodyMotion::
getAxesOfInertia( real t, RealArray & axesOfInertia  ) const
// =======================================================================================
// /Description:
//    Return the axes of inertia at time t.
// /t (input):
// /aCM (output) : axes of inertia at time t.
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  if( axesOfInertia.getLength(0)<3 || axesOfInertia.getLength(1)<3 )
    axesOfInertia.redim(3,3);

  return getCoordinates(t,Overture::nullRealArray(),Overture::nullRealArray(),Overture::nullRealArray(),Overture::nullRealArray(),Overture::nullRealArray(),Overture::nullRealArray(),
                        axesOfInertia); 
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{getAcceleration}} 
int RigidBodyMotion::
getAcceleration( real t, RealArray & aCM  ) const
// =======================================================================================
// /Description:
//    Return the acceleration of the centre of mass.
// /t (input):
// /aCM (output) : acceleration of the centre of mass.
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  if( aCM.getLength(0)<3 )
    aCM.redim(3);

  return getCoordinates(t,Overture::nullRealArray(),Overture::nullRealArray(),Overture::nullRealArray(),Overture::nullRealArray(),Overture::nullRealArray(),aCM);
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{getAcceleration}} 
int RigidBodyMotion::
getAngularAcceleration( real t, RealArray & omegaDot  ) const
// =======================================================================================
// /Description:
//    Return the angular acceleration about the centre of mass with respect to the standard
//  coordinate axes (NOT the axes of inertia).
// /t (input):
// /omegaDot (output) : angular acceleration about (x,y,z) axes.
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  if( omegaDot.getLength(0)<3 )
    omegaDot.redim(3);

  return getCoordinates(t,Overture::nullRealArray(),Overture::nullRealArray(),Overture::nullRealArray(),Overture::nullRealArray(),omegaDot);
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{getPosition}} 
int RigidBodyMotion::
getPosition( real t, RealArray & xCM ) const
// =======================================================================================
// /Description:
//    Return the position of the centre of mass.
// /t (input):
// /xCM (output) :  the position of the centre of mass.
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  if( xCM.getLength(0)<3 )
    xCM.redim(3);
  return getCoordinates(t,xCM);
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{getVelocity}} 
int RigidBodyMotion::
getVelocity( real t, RealArray & vCM  ) const
// =======================================================================================
// /Description:
//    Return the velocity of the centre of mass.
// /t (input):
// /vCM (output) : velocity of the centre of mass.
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  if( vCM.getLength(0)<3 )
    vCM.redim(3);
  return getCoordinates(t,Overture::nullRealArray(),vCM);
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{getAngularVelocities}} 
int RigidBodyMotion::
getAngularVelocities( real t, RealArray & omega  ) const
// =======================================================================================
// /Description:
//    Return the angular velocities at time t. 
// /t (input):
// /omega (output) : angular velocities about the axes of inertia. 
//   For a 2d problem only omega(2) will be nonzero.
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  if( omega.getLength(0)<3 )
    omega.redim(3);
  return getCoordinates(t,Overture::nullRealArray(),Overture::nullRealArray(),Overture::nullRealArray(),omega );
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{getAngularVelocities}} 
int RigidBodyMotion::
getRotationMatrix( real t, 
                   RealArray & r,                     
		   RealArray & rDot /* = Overture::nullRealArray() */,
		   RealArray & rDotDot /* = Overture::nullRealArray()  */) const
// =======================================================================================
// /Description:
//    Return the rotation matrix at time t and optionally the first and second time
// derivative of the rotation matrix.
// /t (input):
// /r (output) : rotation matrix at time t (always 3x3)
// /rDot (output) : if dimensioned on input then on output the 
//     time derivative of the rotation matrix at time t (always 3x3)
// /rDotDot (output) : if dimensioned on input then on output the
//      second time derivative of th rotation matrix at time t (always 3x3)
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  if( r.getLength(0)<3 )
    r.redim(3,3);

  getCoordinates(t,Overture::nullRealArray(),Overture::nullRealArray(),r);

  if( rDot.getLength(0)>=3 && rDot.getLength(1)>=3 )
  {
    // rDot = eDot*e0
    // eDot_i = -omega X e_i 
    RealArray ee(3,3),omega(3),eDot(3,3);
    //                   xCM        vCM             rotation   omega   omegaDot      aCM   axesOfInertia 
    getCoordinates(t,Overture::nullRealArray(),Overture::nullRealArray(),Overture::nullRealArray(),omega,Overture::nullRealArray(),Overture::nullRealArray(),ee);

    // eDot =  w X e
    eDot(0,R)=omega(1)*ee(2,R)-omega(2)*ee(1,R);
    eDot(1,R)=omega(2)*ee(0,R)-omega(0)*ee(2,R);
    eDot(2,R)=omega(0)*ee(1,R)-omega(1)*ee(0,R);

    for( int n=0; n<3; n++ )
      rDot(R,n)=eDot(R,0)*e0(0,n)+eDot(R,1)*e0(1,n)+eDot(R,2)*e0(2,n);

  }
  if( rDotDot.getLength(0)>=3 && rDotDot.getLength(1)>=3 )
  {
    // rDotDot = eDotDot*e0
    // eDotDot_i =  omegaDot X e_i + omega X eDot
    RealArray ee(3,3),omega(3),omegaDot(3),eDot(3,3),eDotDot(3,3);
    //                   xCM        vCM             rotation   omega   omegaDot         aCM   axesOfInertia 
    getCoordinates(t,Overture::nullRealArray(),Overture::nullRealArray(),Overture::nullRealArray(),omega,omegaDot,Overture::nullRealArray(),ee);

    eDot(0,R)=omega(1)*ee(2,R)-omega(2)*ee(1,R);
    eDot(1,R)=omega(2)*ee(0,R)-omega(0)*ee(2,R);
    eDot(2,R)=omega(0)*ee(1,R)-omega(1)*ee(0,R);

    eDotDot(0,R)=omegaDot(1)*ee(2,R)-omegaDot(2)*ee(1,R) + omega(1)*eDot(2,R)-omega(2)*eDot(1,R);
    eDotDot(1,R)=omegaDot(2)*ee(0,R)-omegaDot(0)*ee(2,R) + omega(2)*eDot(0,R)-omega(0)*eDot(2,R);
    eDotDot(2,R)=omegaDot(0)*ee(1,R)-omegaDot(1)*ee(0,R) + omega(0)*eDot(1,R)-omega(1)*eDot(0,R);

    for( int n=0; n<3; n++ )
      rDotDot(R,n)=eDotDot(R,0)*e0(0,n)+eDotDot(R,1)*e0(1,n)+eDotDot(R,2)*e0(2,n);

  }
  

  return 0;
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{getPointTransformationMatrix}} 
int RigidBodyMotion::
getPointTransformationMatrix( real t, 
			      RealArray & rDotRt /* = Overture::nullRealArray() */,
			      RealArray & rDotDotRt /* = Overture::nullRealArray()  */) const
// =======================================================================================
// /Description:
//
//    The velocity and acceleration of a point $\pv(t)$ belonging to a rigid body are
// \begin{align*}
//      \dot{\pv} &= \vv(t) + \dot{R} R^T \pv \\
//      \ddot{\pv} &= \av + \ddot{R} R^T \pv    
// \end{align*}
//  since $R^T=R^{-1}$. This routine can compute $\dot{R} R^T$ and $\ddot{R} R^T$.
// 
// Use this routine if you have to compute the velocity and accelerations of a large number of 
// points.
//
// /t (input):
// /rDotRt (output) : if dimensioned on input, on output this will be transformation matrix
//        $\dot{R} R^T$ for the velocity
// /rDotDotRt (output) : if dimensioned on input, on output this will be transformation matrix 
//    $\ddot{R} R^T$  for the acceleration.
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{

  RealArray r(3,3),rDot(3,3),rDotDot(3,3);
  getRotationMatrix(t,r,rDot,rDotDot);
  
  r=transpose(r);
  if( rDotRt.getLength(0)>=3 && rDotRt.getLength(1)>=3 )
  {
    for( int n=0; n<3; n++ )
      rDotRt(R,n)=rDot(R,0)*r(0,n)+rDot(R,1)*r(1,n)+rDot(R,2)*r(2,n);
  }
  if( rDotDotRt.getLength(0)>=3 && rDotDotRt.getLength(1)>=3 )
  {
    for( int n=0; n<3; n++ )
      rDotDotRt(R,n)=rDotDot(R,0)*r(0,n)+rDotDot(R,1)*r(1,n)+rDotDot(R,2)*r(2,n);
  }

  return 0;
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{getPointVelocity}} 
int RigidBodyMotion::
getPointVelocity( real t, const RealArray & p, RealArray & vp) const
// =======================================================================================
// /Description:
//
//    The velocity of a point $\pv(t)$ belonging to a rigid body is
// \begin{align*}
//      \dot{\pv}(t) &= \vv_\cm(t) + \dot{R} R^T \pv(t) \\
// \end{align*}
// /Note: use {\tt getPointTransformationMatrix} if you need values for many points as this
// routine is not efficient.
// /t (input): evaluate at this time.
// /p (output) : a point at time t.
// /vp (output) : velocity of the point.
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  RealArray vCM(3),rDotRt(3,3);
  
  getVelocity(t,vCM);
  getPointTransformationMatrix(t,rDotRt);

  vp(R)=vCM(R)+rDotRt(R,0)*p(0)+rDotRt(R,1)*p(1)+rDotRt(R,2)*p(2);
  
  return 0;
}

//\begin{>>RigidBodyMotionInclude.tex}{\subsection{getPointAcceleration}} 
int RigidBodyMotion::
getPointAcceleration( real t, const RealArray & p, RealArray & ap) const
// =======================================================================================
// /Description:
//
//    The acceleration of a point $\pv(t)$ belonging to a rigid body is
// \begin{align*}
//      \ddot{\pv}(t) &= \av_\cm(t) + \ddot{R} R^T \pv(t) \\
// \end{align*}
// /Note: use {\tt getPointTransformationMatrix} if you need values for many points as this
// routine is not efficient.
//
// /t (input): evaluate at this time.
// /p (output) : a point at time t.
// /ap (output) : acceleration of the point.
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  RealArray aCM(3),rDotDotRt(3,3);
  
  getAcceleration(t,aCM);
  getPointTransformationMatrix(t,rDotDotRt);

  ap(R)=aCM(R)+rDotDotRt(R,0)*p(0)+rDotDotRt(R,1)*p(1)+rDotDotRt(R,2)*p(2);
  return 0;
}


//\begin{>>RigidBodyMotionInclude.tex}{\subsection{getCoordinates}}
int RigidBodyMotion::
getCoordinates( real t, 
		RealArray & xCM      /* = Overture::nullRealArray() */, 
		RealArray & vCM      /* = Overture::nullRealArray() */,
		RealArray & rotation /* = Overture::nullRealArray() */, 
		RealArray & omega    /* = Overture::nullRealArray() */, 
		RealArray & omegaDot /* = Overture::nullRealArray() */, 
		RealArray & aCM      /* = Overture::nullRealArray() */,
                RealArray & axesOfInertia /* = Overture::nullRealArray() */ ) const
// =======================================================================================
// /Description:
//     Return one or more 'coordinates' of the rigid body. This routine will interpolate (or extrapolate)
//   existing values to obtain a value at time t.
//
// /t (input) : return the coordinates at this time
// /xCM (output) : if this array is dimensioned large enough then it will return the 
//       position of the center of mass at time t.
// /vCM (output) : if this array is dimensioned large enough then it will return the 
//       position of the velocity of mass at time t.
// /rotation (output) : (always 3x3) if this array is dimensioned then determine the
//   rotation matrix for rotating the body from it's initial position.
// /omega (output) : if this array is dimensioned large enough then it will return the 
//       angular velocities. This array always
//        has 3 components. For a 2d problem only omega(2) will be nonzero.
// /omegaDot (output) : if this array is dimensioned large enough then it will return the 
//       angular accelerations. This array always
//        has 3 components. For a 2d problem only omega(2) will be nonzero.
// /aCM (output) : acceleration of the centre of mass
// /axesOfInertia (output) : axes of inertia
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  if( numberSaved<0 )
  {
    printF("RigidBodyMotion::integrate:WARNING:setInitialConditions should be called first\n");
    Overture::abort("error");
  }

  // printf(" >>>RigidBodyMotion::getCoordinates: current=%i\n",current);

  //printf("----GETCOORDINATES: mI(1)=%16.8e, mI(2)=%16.8e\n", mI(1), mI(2));
  
  const bool includeAddedMass = dbase.get<bool>("includeAddedMass");

  // --- we just interpolate a value from the closest two times we have ---
    
  int i=current;
  int ip1;
  real alpha;
  if( numberSaved>=1 )
  {
    for( int j=0; j<numberSaved; j++ )
    {
      if( t<time(i) )
	i=(i-1+maximumNumberToSave) % maximumNumberToSave;
      else
	break;
    }
    if( t<time(i) )
    {
      // no appropriate time was found..
      i=current;
    }
  
    ip1 = (i+1) % maximumNumberToSave;
    alpha = (time(ip1)-t)/max(REAL_MIN,time(ip1)-time(i));
  }
  else
  {
    i=0;
    ip1=i;
    alpha=0.;

    if( t!=0. )
    {
      printF("RigidBodyMotion::getCoordinates:WARNING: no information available at t=%e. numberSaved=%i. "
             "I will use the initial conditions.\n",t,numberSaved);
      printF(" : xCM=[%8.2e,%8.2e,%8.2e] vCM=[%8.2e,%8.2e,%8.2e]"
             " f=[%8.2e,%8.2e,%8.2e] \n",x(0,i),x(1,i),x(2,i),v(0,i),v(1,i),v(2,i),f(0,i),f(1,i),f(2,i));
      
      // OV_ABORT("ERROR");
    }
  }
  
  if( fabs(alpha-.5)>.5 )
  {
    printF("RigidBodyMotion::getPosition: warning, extrapolating value, alpha=%e, t=%e \n",alpha,t);
  }
  if( FALSE )
  {
    printF("RigidBodyMotion::getPosition:alpha=%e, t=%e, i=%i, ip1=%i time(i)=%e, time(ip1)=%e \n",
	   alpha,t,i,ip1,time(i),time(ip1));
  }
  
  if( xCM.getLength(0)>=3 )
    xCM(R)=(1.-alpha)*x(R,ip1)+alpha*x(R,i);

  if( vCM.getLength(0)>=3 )
    vCM(R)=(1.-alpha)*v(R,ip1)+alpha*v(R,i);

  if( rotation.getLength(0)>=3 && rotation.getLength(1)>=3  )
  {
    RealArray r(3,3);
    // rotation matrix = e(t) e^{-1}(0)
    r(R,R)=(1.-alpha)*e(R,R,ip1)+alpha*e(R,R,i);

    int n;
    for( n=0; n<3; n++ )
      rotation(R,n)=r(R,0)*e0(0,n)+r(R,1)*e0(1,n)+r(R,2)*e0(2,n);
  }

  if( axesOfInertia.getLength(0)>=3 && axesOfInertia.getLength(1)>=3  )
    axesOfInertia(R,R)=(1.-alpha)*e(R,R,ip1)+alpha*e(R,R,i);
  
  if( omega.getLength(0)>=3 )
    omega(R)=(1.-alpha)*w(R,ip1)+alpha*w(R,i);

  if( aCM.getLength(0)>=3 )
  {
    if( !dbase.get<bool>("accelerationComputedByDifferencingVelocity") )
    {
      // -- The acceleration is computed from the equations of motion --
      aCM(R)=((1.-alpha)*f(R,ip1)+alpha*f(R,i))/mass;

      if( logFile!=NULL )
	fPrintF(logFile,"getAcceleration: t=%8.2e, mass=%8.2e, a=F/m = (%6.2e,%6.2e,%6.2e), "
               "i=%i, ip1=%i, alpha=%7.1e \n",t,mass,aCM(0),aCM(1),aCM(2),i,ip1,alpha);
    }
    else
    {
      // -- The acceleration is computed by differencing the velocity ---

      // -- Correct the force for the added mass --
      RealArray fv(3);
      
      fv = (1.-alpha)*f(R,ip1)+alpha*f(R,i);  // force without added mass terms

      RealArray A11(R,R), A12(R,R), A21(R,R), A22(R,R);
      getAddedMassMatrices( t, A11 , A12 , A21, A22 );

      RealArray vv(3), wv(3);
      vv = (1.-alpha)*v(R,  ip1)+alpha*v(R,  i);
      wv = (1.-alpha)*w(R,  ip1)+alpha*w(R,  i);

      fv -= mult(A11,vv) + mult(A12,wv);

      aCM(R) = fv*(1./mass);


      // Compute the acceleration from dv/dt (since the mass may be zero)
      RealArray dvdt(3);
      real dt = time(ip1)-time(i);
      if( dt > 0. )
        dvdt=(v(R,ip1)-v(R,i))/dt;
      else
        dvdt=0.;
      
      if( false )
      {
	printF("RigidBodyMotion::getAcceleration: t=%8.2e, mass=%8.2e,  a=F/m=(%6.2e,%6.2e,%6.2e), dv/dt=(%6.2e,%6.2e,%6.2e)\n"
	       " i=%i, ip1=%i, dt=%8.2e, .. .USE dv/dt for addedMass case. \n",
	       t,mass,aCM(0),aCM(1),aCM(2),dvdt(0),dvdt(1),dvdt(2),i,ip1,dt);
      }
      
      if( logFile!=NULL )
      {
	fPrintF(logFile,
                " getAcceleration: t=%8.2e, a=F/m=(%6.2e,%6.2e,%6.2e), dv/dt=(%6.2e,%6.2e,%6.2e)\n"
		"              mass=%8.2e, i=%i, ip1=%i, dt=%8.2e, current=%i, .. .USE dv/dt for addedMass case. \n",
                t,aCM(0),aCM(1),aCM(2),dvdt(0),dvdt(1),dvdt(2),  mass,i,ip1,dt,current);
      }

      real maxDvdt=max(fabs(dvdt));
      real accelerationLimit=1.e6;  // *************************************** FIX ME ********************
      if( maxDvdt>accelerationLimit )
      {
        dvdt=min(accelerationLimit,max(-accelerationLimit,dvdt));
	printF("RigidBodyMotion::***** limiting the acceleration to %8.2e\n",accelerationLimit);
      }
      

      aCM=dvdt;
      
    }
    
  }
  
  if( omegaDot.getLength(0)>=3 )
  {
//      if( numberOfDimensions!=2 )
//      {
//        printf("RigidBodyMotion::getAngularAcceleration: not implemented yet in 3d\n");
//        Overture::abort("error");
//      }
    
    // angular-acceleration : 

    for( int axis=0; axis<3; axis++ )
    {
      const int axisp1=(axis+1) % 3;
      const int axisp2=(axis+2) % 3;
      if( true )
      {
	// *new way* 2012/01/16

	//  h' = g, h =A*w,  A= E*Lambda*E^T, A^{-1} = E*Lambda^{-1}*E^T
	//  A * wDot = - Omega*A*w  + g 

	RealArray ea(3,3), Lambda(3,3), A(3,3), Ai(3,3), Omega(3,3), wv(3), gv(3);
	Lambda=0.;
	Lambda(0,0)=mI(0); Lambda(1,1)=mI(1); Lambda(2,2)=mI(2);

        // Get E, w and g at time t 
	ea = (1.-alpha)*e(R,R,ip1)+alpha*e(R,R,i);          // *FIX ME* for higher order accuracy 
        wv = (1.-alpha)*w(R,  ip1)+alpha*w(R,  i);
	gv = (1.-alpha)*g(R,  ip1)+alpha*g(R,  i);
	
	A  = mult(ea,mult(Lambda,trans(ea)));

        Lambda(0,0)=1./mI(0); Lambda(1,1)=1./mI(1); Lambda(2,2)=1./mI(2); // LambdaInverse
	Ai = mult(ea,mult(Lambda,trans(ea)));  // A^{-1}

	Omega = getCrossProductMatrix( wv );

	if( includeAddedMass )
	{
          // -- include added mass terms --
	  RealArray A11(R,R), A12(R,R), A21(R,R), A22(R,R);
	  getAddedMassMatrices( t, A11 , A12 , A21, A22 );

          RealArray vv(3);
          vv = (1.-alpha)*v(R,  ip1)+alpha*v(R,  i);
          gv -= mult(A12,vv) + mult(A22,wv);
	}
	

	omegaDot = mult( Ai, evaluate(- mult(Omega,mult(A,wv)) + gv) );

      }
      else
      {
	// *old way*
	omegaDot(axis)=( 
	  (1.-alpha)*( (mI(axisp1)-mI(axisp2))*w(axisp1,ip1)*w(axisp2,ip1)+
		       g(0,ip1)*e(0,axis,ip1)+
		       g(1,ip1)*e(1,axis,ip1)+
		       g(2,ip1)*e(2,axis,ip1))
	  +alpha *( (mI(axisp1)-mI(axisp2))*w(axisp1,i)*w(axisp2,i)+
		    g(0,i)*e(0,axis,i)+
		    g(1,i)*e(1,axis,i)+
		    g(2,i)*e(2,axis,i))
	  )/mI(axis);
      }
    }
    if( dbase.get<bool>("accelerationComputedByDifferencingVelocity") )
    {
      // *NOTE* The torque, gv, is NOT correct in the added mass case since we are missing the z*vb term!

      // Compute the angular acceleration from d(omega)/dt (since the moments of inertia may be zero)
      RealArray dwdt(3);
      real dt = time(ip1)-time(i);
      if( dt > 0. )
        dwdt=(w(R,ip1)-w(R,i))/dt;
      else
        dwdt=0.;
      
      if( false )
      {
	printF("RigidBodyMotion::getOmegaDot: t=%8.2e, mI=(%8.2e,%8.2e,%8.2e), wDot=(%6.2e,%6.2e,%6.2e)," 
	       " dw/dt=(%6.2e,%6.2e,%6.2e)\n"
	       " i=%i, ip1=%i, dt=%8.2e, current=%i, .. .USE dw/dt for addedMass case. \n",
	       t,mI(0),mI(1),mI(2),omegaDot(0),omegaDot(1),omegaDot(2),dwdt(0),dwdt(1),dwdt(2),i,ip1,dt,current);
      }
      
      if( logFile!=NULL )
      {
	fPrintF(logFile,
                " getOmegaDot: t=%8.2e, wDot=(%6.2e,%6.2e,%6.2e), dw/dt=(%6.2e,%6.2e,%6.2e)\n"
		"              mI=(%8.2e,%8.2e,%8.2e), i=%i, ip1=%i, dt=%8.2e, current=%i, .. .USE dw/dt for addedMass case. \n",
		t,omegaDot(0),omegaDot(1),omegaDot(2),dwdt(0),dwdt(1),dwdt(2),mI(0),mI(1),mI(2),i,ip1,dt,current);
      }
      

      real maxDwdt=max(fabs(dwdt));
      real accelerationLimit=1.e6;  // *************************************** FIX ME ********************
      if( maxDwdt>accelerationLimit )
      {
        dwdt=min(accelerationLimit,max(-accelerationLimit,dwdt));
	printF("RigidBodyMotion::***** limiting the angular acceleration wDot to (%8.2e,%8.2e,%8.2e)\n",dwdt(0),dwdt(1),dwdt(2));
      }

      omegaDot=dwdt;  // ****************
      
    }
    else
    {
      if( logFile!=NULL )
      {
	fPrintF(logFile,
                " getOmegaDot: t=%8.2e, wDot=(%6.2e,%6.2e,%6.2e), mI=(%8.2e,%8.2e,%8.2e)\n",
		t,omegaDot(0),omegaDot(1),omegaDot(2),mI(0),mI(1),mI(2));
      }
    }
    

  }
  
  return 0;
}

// =======================================================================================================
/// \brief: Return the force and torque at time t (interpolate in time if necessary). 
/// \param t (input) : evaluate the force and torque at this time.
/// \param fv, gv (output) : force and torque
// =======================================================================================================
int RigidBodyMotion::
getForce(const real t, RealArray & fv, RealArray & gv ) const
{
  
  assert( current>=0 );
  const int previous = (current-1+maximumNumberToSave) % maximumNumberToSave;
  const int next = (current+1) % maximumNumberToSave;

  const int orderOfAccuracy = dbase.get<int>("orderOfAccuracy");

  assert( numberSaved>=1 );

  // The user may have supplied values of the forcings at negative times.
  const int numberOfForcingsSaved = numberSaved + dbase.get<int>("numberOfPastTimeValues");

  // Linear interpolation in time:
  if( orderOfAccuracy<=2 || numberOfForcingsSaved<=1 ) // Note: numberSaved does not include solution "next"
  {
    // Linear interpolation in time:
    real t1=time(next), t2=time(current);
    real c1 = (t-t2)/(t1-t2);
    real c2 = (t-t1)/(t2-t1);
    fv= c1*f(R,next) + c2*f(R,current);
    gv= c1*g(R,next) + c2*g(R,current);
      
  }
  else if( orderOfAccuracy==3 || numberOfForcingsSaved<=2 )
  {
    // Quadratic Lagrange interpolation in time
    real t1=time(next), t2=time(current), t3=time(previous);
    real c1 = ( (t-t2)*(t-t3) )/( (t1-t2)*(t1-t3) );
    real c2 = ( (t-t3)*(t-t1) )/( (t2-t3)*(t2-t1) );
    real c3 = ( (t-t1)*(t-t2) )/( (t3-t1)*(t3-t2) );
      

    fv= c1*f(R,next) + c2*f(R,current)+ c3*f(R,previous);
    gv= c1*g(R,next) + c2*g(R,current)+ c3*g(R,previous);

  }

  else if( orderOfAccuracy==4 )
  {
    // Cubic Lagrange interpolation in time, use 4 time levels
    const int prev2 = (current-2+maximumNumberToSave) % maximumNumberToSave;

    real t1=time(next), t2=time(current), t3=time(previous), t4=time(prev2);
    real c1 = ( (t-t2)*(t-t3)*(t-t4) )/( (t1-t2)*(t1-t3)*(t1-t4) );
    real c2 = ( (t-t3)*(t-t4)*(t-t1) )/( (t2-t3)*(t2-t4)*(t2-t1) );
    real c3 = ( (t-t4)*(t-t1)*(t-t2) )/( (t3-t4)*(t3-t1)*(t3-t2) );
    real c4 = ( (t-t1)*(t-t2)*(t-t3) )/( (t4-t1)*(t4-t2)*(t4-t3) );
      
    fv= c1*f(R,next) + c2*f(R,current)+ c3*f(R,previous)+ c4*f(R,prev2);
    gv= c1*g(R,next) + c2*g(R,current)+ c3*g(R,previous)+ c4*g(R,prev2);

  }
  else
  {
    printF("RigidBodyMotion::getForceInternal:ERROR: interpolation for orderOfAccuracy=%i not implemented\n",orderOfAccuracy);
    OV_ABORT("ERROR");
  }

  const bool includeAddedMass = dbase.get<bool>("includeAddedMass");
  if( includeAddedMass )
  {
    // include added mass terms:

    RealArray A11(R,R), A12(R,R), A21(R,R), A22(R,R);
    getAddedMassMatrices( t, A11 , A12 , A21, A22 );

    RealArray v0(3), w0(3);
    getVelocity(t,v0);
    getAngularVelocities(t,w0);

    fv += mult(A11,v0) + mult(A12,w0);
    gv += mult(A21,v0) + mult(A22,w0);
  }
  
  return 0;

}


//\begin{>>RigidBodyMotionInclude.tex}{\subsection{get}} 
int RigidBodyMotion::
get( const GenericDataBase & dir, const aString & name)
// =======================================================================================
// /Description:
//  Read parameters to a data-base file
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"RigidBodyMotion");

  aString className;
  subDir.get( className,"className" ); 

  subDir.get(mass,"mass"); 
  subDir.get(density,"density"); 
  subDir.get(numberOfDimensions,"numberOfDimensions"); 
  subDir.get(current,"current"); 
  subDir.get(numberOfSteps,"numberOfSteps"); 
  subDir.get(numberSaved,"numberSaved"); 
  subDir.get(maximumNumberToSave,"maximumNumberToSave"); 
  subDir.get(initialConditionsGiven,"initialConditionsGiven"); 

  subDir.get(mI,"mI"); 
  subDir.get(e,"e");  // time history 
  subDir.get(e0,"e0"); 
  subDir.get(x,"x");  // time history 
  subDir.get(v,"v");  // time history 
  subDir.get(x0,"x0"); 
  subDir.get(v0,"v0"); 
  subDir.get(w0,"w0"); 
  subDir.get(f,"f");    // time history 
  subDir.get(g,"g");    // time history 
  subDir.get(w,"w");    // time history 
  subDir.get(time,"time");  // time history 
  subDir.get(bodyForceCoeff,"bodyForceCoeff"); 
  subDir.get(bodyTorqueCoeff,"bodyTorqueCoeff"); 

  int temp;
  subDir.get(temp,"positionConstraint");  positionConstraint=(PositionConstraintEnum)temp;
  subDir.get(temp,"rotationConstraint");  rotationConstraint=(RotationConstraintEnum)temp;
  subDir.get(constraintValues,"constraintValues"); 
  
  subDir.get(relaxCorrectionSteps,"relaxCorrectionSteps");
  // bool correctionHasConverged;
  // real maximumRelativeCorrection;
  subDir.get(correctionAbsoluteToleranceForce,"correctionAbsoluteToleranceForce");
  subDir.get(correctionRelativeToleranceForce,"correctionRelativeToleranceForce");
  subDir.get(correctionRelaxationParameterForce,"correctionRelaxationParameterForce");
  
  subDir.get(correctionAbsoluteToleranceTorque,"correctionAbsoluteToleranceTorque");
  subDir.get(correctionRelativeToleranceTorque,"correctionRelativeToleranceTorque");
  subDir.get(correctionRelaxationParameterTorque,"correctionRelaxationParameterTorque");
  
  subDir.get(twilightZone,"twilightZone");
  subDir.get(temp,"twilightZoneType"); twilightZoneType=TwilightZoneTypeEnum(temp);

  subDir.get(dbase.get<int>("orderOfAccuracy"),"orderOfAccuracy");
  subDir.get(dbase.get<bool>("includeAddedMass"),"includeAddedMass");
  bool addedMassExists=false; // this is true if the AddedMass matrix exists 
  subDir.get(addedMassExists,"addedMassExists");
  if( addedMassExists )
  {
    if( !dbase.has_key("AddedMass" ) )
      dbase.put<RealArray>("AddedMass");
    subDir.get(dbase.get<RealArray>("AddedMass"),"AddedMass");  // time history of the added mass matrices
  }
  
  subDir.get(dbase.get<real>("toleranceNewton"),"toleranceNewton");
  subDir.get(dbase.get<int>("numberOfPastTimeValues"),"numberOfPastTimeValues");

  subDir.get(dbase.get<bool>("useExtrapolationInPredictor"),"useExtrapolationInPredictor");
  subDir.get(dbase.get<int>("orderOfExtrapolationPredictor"),"orderOfExtrapolationPredictor");
  
  subDir.get(dbase.get<bool>("accelerationComputedByDifferencingVelocity"),
                             "accelerationComputedByDifferencingVelocity");
  
  subDir.get(temp,"bodyForceType");   bodyForceType=(BodyForceTypeEnum)temp;

  subDir.get(temp,"timeSteppingMethod");   timeSteppingMethod=(TimeSteppingMethodEnum)temp;
  
  // printf(" >>>RigidBodyMotion::get: current=%i\n",current);

  delete &subDir;
  return 0;
}


//\begin{>>RigidBodyMotionInclude.tex}{\subsection{putt}} 
int RigidBodyMotion::
put( GenericDataBase & dir, const aString & name) const
// ======================================================================================
// /Description: 
//    Save parameters to a data-base file
//\end{RigidBodyMotionInclude.tex}  
// ======================================================================================
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"RigidBodyMotion");                 // create a sub-directory 

  aString className="RigidBodyMotion";
  subDir.put( className,"className" );

  subDir.put(mass,"mass"); 
  subDir.put(density,"density"); 
  subDir.put(numberOfDimensions,"numberOfDimensions"); 
  subDir.put(current,"current"); 
  subDir.put(numberOfSteps,"numberOfSteps"); 
  subDir.put(numberSaved,"numberSaved"); 
  subDir.put(maximumNumberToSave,"maximumNumberToSave"); 
  subDir.put(initialConditionsGiven,"initialConditionsGiven"); 

  subDir.put(mI,"mI"); 
  subDir.put(e,"e");              // time history 
  subDir.put(e0,"e0"); 
  subDir.put(x,"x");             // time history
  subDir.put(v,"v");             // time history 
  subDir.put(x0,"x0"); 
  subDir.put(v0,"v0"); 
  subDir.put(w0,"w0"); 
  subDir.put(f,"f");             // time history 
  subDir.put(g,"g");             // time history 
  subDir.put(w,"w");             // time history 
  subDir.put(time,"time");       // time history

  subDir.put(bodyForceCoeff,"bodyForceCoeff"); 
  subDir.put(bodyTorqueCoeff,"bodyTorqueCoeff"); 

  subDir.put(positionConstraint,"positionConstraint");  
  subDir.put(rotationConstraint,"rotationConstraint");  
  subDir.put(constraintValues,"constraintValues"); 

  subDir.put(relaxCorrectionSteps,"relaxCorrectionSteps");
  // bool correctionHasConverged;
  // real maximumRelativeCorrection;
  subDir.put(correctionAbsoluteToleranceForce,"correctionAbsoluteToleranceForce");
  subDir.put(correctionRelativeToleranceForce,"correctionRelativeToleranceForce");
  subDir.put(correctionRelaxationParameterForce,"correctionRelaxationParameterForce");
  
  subDir.put(correctionAbsoluteToleranceTorque,"correctionAbsoluteToleranceTorque");
  subDir.put(correctionRelativeToleranceTorque,"correctionRelativeToleranceTorque");
  subDir.put(correctionRelaxationParameterTorque,"correctionRelaxationParameterTorque");

  subDir.put(twilightZone,"twilightZone");
  subDir.put((int)twilightZoneType,"twilightZoneType");

  subDir.put(dbase.get<int>("orderOfAccuracy"),"orderOfAccuracy");
  subDir.put(dbase.get<bool>("includeAddedMass"),"includeAddedMass");
  const bool addedMassExists=dbase.has_key("AddedMass");
  subDir.put(addedMassExists,"addedMassExists");
  if( addedMassExists )
    subDir.put(dbase.get<RealArray>("AddedMass"),"AddedMass");  // time history of the added mass matrices
  subDir.put(dbase.get<real>("toleranceNewton"),"toleranceNewton");
  subDir.put(dbase.get<int>("numberOfPastTimeValues"),"numberOfPastTimeValues");

  subDir.put(dbase.get<bool>("useExtrapolationInPredictor"),"useExtrapolationInPredictor");
  subDir.put(dbase.get<int>("orderOfExtrapolationPredictor"),"orderOfExtrapolationPredictor");

  subDir.put(dbase.get<bool>("accelerationComputedByDifferencingVelocity"),
                             "accelerationComputedByDifferencingVelocity");

  subDir.put((int)bodyForceType,"bodyForceType"); 

  subDir.put(timeSteppingMethod,"timeSteppingMethod"); 

  delete &subDir;
  return 0;  
}

// ===================================================================================================================
/// \brief Build the plot options dialog.
/// \param dialog (input) : graphics dialog to use.
///
// ==================================================================================================================
int RigidBodyMotion::
buildBodyForceOptionsDialog(DialogData & dialog )
{

  aString cmds[] = {"help body force",
                    "body force x time function...",
                    "body force y time function...",
                    "body force z time function...",
                    "body torque x time function...",
                    "body torque y time function...",
                    "body torque z time function...",
		    ""};
  int numberOfPushButtons=7;  // number of entries in cmds
  int numRows= (numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

//   aString cmd[] = {"plot and wait first time", "plot with no waiting",
// 		   "plot and always wait","no plotting","" };

  aString optionCmd[]={ "time polynomial",
                        "time function",
                        ""}; // 
  dialog.addOptionMenu("Type:", optionCmd, optionCmd, (int)bodyForceType);


  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textCommands[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textCommands[nt] = "body force x:";  
  sPrintF(textStrings[nt], "%g, %g, %g, %g (coeffs of time poly)",
          bodyForceCoeff(0,0),bodyForceCoeff(0,1),bodyForceCoeff(0,2),bodyForceCoeff(0,3));  nt++; 
    
  textCommands[nt] = "body force y:";  
  sPrintF(textStrings[nt], "%g, %g, %g, %g (coeffs of time poly)",
          bodyForceCoeff(1,0),bodyForceCoeff(1,1),bodyForceCoeff(1,2),bodyForceCoeff(1,3));  nt++; 
    
  textCommands[nt] = "body force z:";  
  sPrintF(textStrings[nt], "%g, %g, %g, %g (coeffs of time poly)",
          bodyForceCoeff(2,0),bodyForceCoeff(2,1),bodyForceCoeff(2,2),bodyForceCoeff(2,3));  nt++; 
    
  textCommands[nt] = "body torque x:";  
  sPrintF(textStrings[nt], "%g, %g, %g, %g (coeffs of time poly)",
          bodyTorqueCoeff(0,0),bodyTorqueCoeff(0,1),bodyTorqueCoeff(0,2),bodyTorqueCoeff(0,3));  nt++; 
    
  textCommands[nt] = "body torque y:";  
  sPrintF(textStrings[nt], "%g, %g, %g, %g (coeffs of time poly)",
          bodyTorqueCoeff(1,0),bodyTorqueCoeff(1,1),bodyTorqueCoeff(1,2),bodyTorqueCoeff(1,3));  nt++; 
    
  textCommands[nt] = "body torque z:";  
  sPrintF(textStrings[nt], "%g, %g, %g, %g (coeffs of time poly)",
          bodyTorqueCoeff(2,0),bodyTorqueCoeff(2,1),bodyTorqueCoeff(2,2),bodyTorqueCoeff(2,3));  nt++; 
    


  // null strings terminal list
  textCommands[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textCommands, textCommands, textStrings);


  return 0;
}

//================================================================================
/// \brief: Look for a plot option in the string "answer"
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int RigidBodyMotion::
getBodyForceOption(const aString & answer,
		   DialogData & dialog )
{
  // GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

  bool found=true;
  int len=0;

  if( answer.matches("body force x:") ||
      answer.matches("body force y:") ||
      answer.matches("body force z:") ||
      answer.matches("body torque x:") ||
      answer.matches("body torque y:") ||
      answer.matches("body torque z:") )
  {
    Range P=4;  // number of polynomial coeff.
    RealArray bf(Range(1),P); bf=0.;
    if( answer.matches("body force" ) )
      len=13;
    else
      len=14;
    
    sScanF(answer(len,answer.length()-1),"%e %e %e %e",&bf(0,0),&bf(0,1),&bf(0,2),&bf(0,3));
    if( answer.matches("body force x:") )
      bodyForceCoeff(0,P)=bf;
    else if( answer.matches("body force y:") )
      bodyForceCoeff(1,P)=bf;
    else if( answer.matches("body force z:") )
      bodyForceCoeff(2,P)=bf;
    else if( answer.matches("body torque x:") )
      bodyTorqueCoeff(0,P)=bf;
    else if( answer.matches("body torque y:") )
      bodyTorqueCoeff(1,P)=bf;
    else if( answer.matches("body torque z:") )
      bodyTorqueCoeff(2,P)=bf;
    else
    {
      OV_ABORT("ERROR: this should not happen");
    }
    dialog.setTextLabel(answer(0,len-1),sPrintF("%g, %g, %g, %g (coeffs in time poly)",
						bf(0,0),bf(0,1),bf(0,2),bf(0,3)));
  }
  else if( answer=="time polynomial" ||
           answer=="time function" )
  {
    bodyForceType= answer=="time polynomial" ? timePolynomialBodyForce : timeFunctionBodyForce;
  }
  else if( answer=="body force x time function..." ||
	   answer=="body force y time function..." ||
	   answer=="body force z time function..." ||
	   answer=="body torque x time function..." ||
	   answer=="body torque y time function..." ||
	   answer=="body torque z time function..." )
  {
    aString timeFunctionName;
    timeFunctionName = ( answer=="body force x time function..." ? "bodyForceX" :
			 answer=="body force y time function..." ? "bodyForceY" :
			 answer=="body force z time function..." ? "bodyForceZ" :
			 answer=="body torque x time function..." ? "bodyTorqueX" :
			 answer=="body torque y time function..." ? "bodyTorqueY" :
			 answer=="body torque z time function..." ? "bodyTorqueZ" : "unknown" );
    assert( timeFunctionName!="unknown" );

    // Change parameters in the Time function
    if( !dbase.has_key(timeFunctionName) )
    {
      dbase.put<TimeFunction>((const char*)timeFunctionName,TimeFunction());
    }
    TimeFunction & timeFunction = dbase.get<TimeFunction>((const char*)timeFunctionName);
    GenericGraphicsInterface & gi = *Overture::getGraphicsInterface();
    printF("Edit the time funcion %s...\n",(const char*)timeFunctionName);
    timeFunction.update(gi);
  }
  else if( answer=="help body force" )
  {
    printF("The body force can be defined as a simple polynomial in time,\n"
           " or using the TimeFunction class to define more general functions in time.\n");
    if( bodyForceType==timePolynomialBodyForce )
    {
      printF("The body force and body torque can be defined a polynomials in time.\n"
	     "Here are the current values:\n");
      for( int axis=0; axis<3; axis++ )
	printF(" bodyForce%s  = %g + %g*t + %g*t^2 + %g*t^3\n",(axis==0 ? "X" : axis==1 ? "Y" : "Z"),
	       bodyForceCoeff(axis,0),bodyForceCoeff(axis,1),
	       bodyForceCoeff(axis,2),bodyForceCoeff(axis,3));
      for( int axis=0; axis<3; axis++ )
	printF(" bodyTorque%s = %g + %g*t + %g*t^2 + %g*t^3\n",(axis==0 ? "X" : axis==1 ? "Y" : "Z"),
	       bodyTorqueCoeff(axis,0),bodyTorqueCoeff(axis,1),
	       bodyTorqueCoeff(axis,2),bodyTorqueCoeff(axis,3));
    }
    else
    {
      printF("The body force and body torque are defined using TimeFunctions.\n");
    }
  }
  else
  {
    found=false;
  }
  

  return found;
}


//\begin{>>RigidBodyMotionInclude.tex}{\subsection{update}} 
int RigidBodyMotion::
update( GenericGraphicsInterface & gi )
// =======================================================================================
// /Description:
// 
//\end{RigidBodyMotionInclude.tex}  
//=========================================================================================
{
  bool & useExtrapolationInPredictor = dbase.get<bool>("useExtrapolationInPredictor");
  int & orderOfExtrapolationPredictor = dbase.get<int>("orderOfExtrapolationPredictor");

  bool & accelerationComputedByDifferencingVelocity = dbase.get<bool>("accelerationComputedByDifferencingVelocity");
  
  aString answer,answer2;
  char buff[80];


  aString menu[] =
  {
    "!Rigid body parameters",
    "mass",
    "density",
    "moments of inertia",
    "axes of inertia",
    "initial centre of mass",
    "initial velocity",
    "initial angular velocity",
    "body force",
     //   "body torque",
    ">constraints",
    "position has no constraint",
    "position is constrained to a plane",
    "position is constrained to a line",
    "position is fixed",
    "rotation has no constraint",
    "rotation is constrained to a plane",
    "rotation is constrained to a line",
    "rotation is fixed",
    "<done",
    "exit",
    ""
  };
  
  GUIState dialog;
  dialog.setWindowTitle("RigidBodyMotion");
  dialog.setExitCommand("exit", "exit");

  aString opCommand2[] = {"leapFrogTrapezoidal",
                          "improvedEuler",
                          "implicitRungeKutta",
			  ""};

  dialog.setOptionMenuColumns(1);
  dialog.addOptionMenu( "Method:", opCommand2, opCommand2, (int)timeSteppingMethod );


//   aString colourBoundaryCommands[] = { "colour by bc",
// 			               "colour by share",
// 			               "" };
//   // dialog.addRadioBox("boundaries:",colourBoundaryCommands, colourBoundaryCommands, 0 );
//   dialog.addOptionMenu("boundaries:",colourBoundaryCommands, colourBoundaryCommands, 0 );

  aString cmds[] = {"body force...",
		    ""};
  int numberOfPushButtons=1;  // number of entries in cmds
  int numRows=numberOfPushButtons; // (numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  aString tbCommands[] = {"relax correction steps",
                          "added mass",
 			  "use extrapolation in predictor",
			  "acceleration from differences",
			  ""};
  int tbState[10];

  tbState[0] = relaxCorrectionSteps;
  tbState[1] = dbase.get<bool>("includeAddedMass");
  tbState[2] = useExtrapolationInPredictor;
  tbState[3] = accelerationComputedByDifferencingVelocity;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);


  const int numberOfTextStrings=15;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];


  int nt=0;
  textLabels[nt] = "debug:";  sPrintF(textStrings[nt],"%i",debug);  
  nt++; 

  textLabels[nt] = "force relaxation parameter:";  sPrintF(textStrings[nt],"%g",correctionRelaxationParameterForce);  
  nt++; 

  textLabels[nt] = "force relative tol:";  sPrintF(textStrings[nt],"%g",correctionRelativeToleranceForce); 
  nt++; 

  textLabels[nt] = "force absolute tol:";  sPrintF(textStrings[nt],"%g",correctionAbsoluteToleranceForce); 
  nt++; 

  textLabels[nt] = "torque relaxation parameter:";  sPrintF(textStrings[nt],"%g",correctionRelaxationParameterTorque); 
  nt++; 
  textLabels[nt] = "torque relative tol:";  sPrintF(textStrings[nt],"%g",correctionRelativeToleranceTorque); 
  nt++; 
  textLabels[nt] = "torque absolute tol:";  sPrintF(textStrings[nt],"%g",correctionAbsoluteToleranceTorque); 
  nt++; 

  textLabels[nt] = "order of accuracy:"; 
  sPrintF(textStrings[nt],"%i",dbase.get<int>("orderOfAccuracy"));  nt++; 

  textLabels[nt] = "newtonTol:"; 
  sPrintF(textStrings[nt],"%8.2e",dbase.get<real>("toleranceNewton"));  nt++; 

  aString logFileName="rigidBody1.log";
  textLabels[nt] = "log file:";  sPrintF(textStrings[nt],"%s",(const char*)logFileName);  
  nt++; 

  textLabels[nt] = "predictor extrap order:"; 
  sPrintF(textStrings[nt],"%i",orderOfExtrapolationPredictor); nt++;

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);


  // add old popup menu
  dialog.buildPopup(menu);
  gi.pushGUI(dialog);


  // Body force dialog: 
  DialogData & bodyForceOptionsDialog = dialog.getDialogSibling();

  bodyForceOptionsDialog.setWindowTitle("BodyForce Options");
  bodyForceOptionsDialog.setExitCommand("close bodyForce options", "close");
  buildBodyForceOptionsDialog(bodyForceOptionsDialog);


  gi.appendToTheDefaultPrompt("rigidBody>");
  for( ;; )
  {

    // gi.getMenuItem(menu,answer,"choose an option");
    gi.getAnswer(answer,"");  
 
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="leapFrogTrapezoidal" ||
             answer=="improvedEuler" ||
	     answer=="implicitRungeKutta" )
    {
      if( answer=="leapFrogTrapezoidal" )
      {
	timeSteppingMethod=leapFrogTrapezoidal;
	printF("RigidBodyMotion: timeSteppingMethod=leapFrogTrapezoidal\n");
      }
      else if( answer=="improvedEuler" )
      {
	timeSteppingMethod=improvedEuler;
        printF("RigidBodyMotion: timeSteppingMethod=improvedEuler\n");
      }
      else if( answer=="implicitRungeKutta" )
      {
	timeSteppingMethod=implicitRungeKutta;
	printF("RigidBodyMotion: timeSteppingMethod=implicitRungeKutta\n");
      }
      else
      {
	OV_ABORT("ERROR");
      }
      dialog.getOptionMenu("Method:").setCurrentChoice(answer);
    }
    else if( answer=="mass" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the body mass (default=%8.2e)",mass));
      if( answer2!="" )
	sScanF(answer2,"%e",&mass);

      setMass(mass);
      
    }
    else if( answer=="density" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the body density, -1. if not known, (default=%8.2e)",density));
      if( answer2!="" )
	sScanF(answer2,"%e",&density);
    }
    else if( answer=="moments of inertia" )
    {
      if( numberOfDimensions==2 )
      {
	gi.inputString(answer2,sPrintF(buff,"Enter the moment of inertia (default=%8.2e)",mI(2)));
	if( answer2!="" )
	  sScanF(answer2,"%e",&mI(2));
      }
      else
      {
	gi.inputString(answer2,sPrintF(buff,"Enter the moments of inertia (default=%8.2e,%8.2e,%8.2e)",
             mI(0),mI(1),mI(2)));
	if( answer2!="" )
	  sScanF(answer2,"%e %e %e",&mI(0),&mI(1),&mI(2));
      }
    }
    else if( answer=="initial centre of mass" || answer=="initial center of mass" )
    {
      if( numberOfDimensions==2 )
      {
	gi.inputString(answer2,sPrintF(buff,"Enter the centre of mass (default=%8.2e,%8.2e)",x0(0),x0(1)));
	if( answer2!="" )
	  sScanF(answer2,"%e %e",&x0(0),&x0(1));
      }
      else
      {
	gi.inputString(answer2,sPrintF(buff,"Enter the centre of mass (default=%8.2e,%8.2e,%8.2e)",
             x0(0),x0(1),x0(2)));
	if( answer2!="" )
	  sScanF(answer2,"%e %e %e",&x0(0),&x0(1),&x0(2));
      }
      setInitialCentreOfMass(x0);
      printF("MovingGrids: >>>>  centerOfMassHasBeenInitialized()=%i \n", centerOfMassHasBeenInitialized()); 
    }
    else if( answer=="initial velocity" )
    {
      if( numberOfDimensions==2 )
      {
	gi.inputString(answer2,sPrintF(buff,"Enter the initial velocity (default=%8.2e,%8.2e)",v0(0),v0(1)));
	if( answer2!="" )
	  sScanF(answer2,"%e %e",&v0(0),&v0(1));
      }
      else
      {
	gi.inputString(answer2,sPrintF(buff,"Enter the initial velocity (default=%8.2e,%8.2e,%8.2e)",
             v0(0),v0(1),v0(2)));
	if( answer2!="" )
	  sScanF(answer2,"%e %e %e",&v0(0),&v0(1),&v0(2));
      }
      v(R,0)=v0(R);
    }
    else if( answer=="initial angular velocity" )
    {
      if( numberOfDimensions==2 )
      {
	gi.inputString(answer2,sPrintF(buff,"Enter the initial angular momentum (default=%8.2e)",w0(2)));
	if( answer2!="" )
	  sScanF(answer2,"%e",&w0(2));
      }
      else
      {
	gi.inputString(answer2,sPrintF(buff,"Enter the initial angular momentum (default=%8.2e,%8.2e,%8.2e)",
             w0(0),w0(1),w0(2)));
	if( answer2!="" )
	  sScanF(answer2,"%e %e %e",&w0(0),&w0(1),&w0(2));
      }
      w(R,0)=w0(R);
    }
    else if( answer=="axes of inertia" )
    {
      if( numberOfDimensions==2 )
      {
	printF("No need to specify axes of inertia in 2D\n");
      }
      else
      {
        for( int axis=0; axis<numberOfDimensions; axis++ )
	{
	  gi.inputString(answer2,sPrintF(buff,"Enter axis %i (default=%8.2e,%8.2e,%8.2e)",
					 axis,e(0,axis),e(1,axis),e(2,axis)));
	  if( answer2!="" )
	    sScanF(answer2,"%e %e %e",&e(0,axis),&e(1,axis),&e(2,axis));
 
          // check that axes are orthogonal and unit length.
          real norm =max(REAL_MIN, SQRT( SQR(e(0,axis))+SQR(e(1,axis))+SQR(e(2,axis)) ));
          e(R,axis)/=norm;
	  for( int dir=0; dir<numberOfDimensions-1; dir++ )
	  {
            real dot = e(0,axis)*e(0,dir)+e(1,axis)*e(1,dir)+e(2,axis)*e(2,dir);
	    if( fabs(dot)>REAL_EPSILON*10. )
	    {
	      printF("ERROR: axis of inertia %i is not orthogonal to axis %i. dot-product=%e \n",axis,dir,dot);
	    }
	  }
	}
      }
    }
    else if( answer=="position has no constraint" )
    {
      positionConstraint=positionHasNoConstraint;
    }
    else if( answer=="position is constrained to a plane" )
    {
       positionConstraint=positionConstrainedToAPlane;
       for( int k=0; k<=1; k++ )
       {
	 RealArray v(3); 
	 v=0;  v(k)=1.;
	 printF("Enter tangent %i to the constraining plane.\n",k);
	 gi.inputString(answer2,sPrintF(buff,"Enter tangent %i to the constraining plane (default=%8.2e,%8.2e,%8.2e)",
					k,v(0),v(1),v(2)));
	 if( answer2!="" )
	   sScanF(answer2,"%e %e %e",&v(0),&v(1),&v(2));

	 real norm = sqrt(sum(SQR(v)));
	 v/=max(REAL_MIN*100.,norm);
	 constraintValues(Range(3+3*k,5+3*k))=v;
      
	 printF(" Constraining plane: normalized tangent %i is (%7.4f,%7.4,%7.4f)\n",k,v(0),v(1),v(2));
       }
       
    }
    else if( answer=="position is constrained to a line" )
    {
       positionConstraint=positionConstrainedToALine;

      RealArray v(3); 
      v=0;  v(0)=1.;
      printF("Enter the tangent to the constraining line.\n");
      gi.inputString(answer2,sPrintF(buff,"Enter the tangent to the constraining line (default=%8.2e,%8.2e,%8.2e)",
				     v(0),v(1),v(2)));
      if( answer2!="" )
	sScanF(answer2,"%e %e %e",&v(0),&v(1),&v(2));

      real norm = sqrt(sum(SQR(v)));
      v/=max(REAL_MIN*100.,norm);
      constraintValues(Range(3,5))=v;
      
      printF(" Constraining line has a normalized tangent of (%7.4f,%7.4f,%7.4f)\n",v(0),v(1),v(2));
    }
    else if( answer=="position is fixed" )
    {
       positionConstraint=positionIsFixed;
    }
    else if( answer=="rotation has no constraint" )
    {
       rotationConstraint=rotationHasNoConstraint;
    }
    else if( answer=="rotation is constrained to a plane" )
    {
       rotationConstraint=rotationConstrainedToAPlane;
       for( int k=0; k<=1; k++ )
       {
	 RealArray v(3); 
	 v=0;  v(k)=1.;
	 printF("Enter tangent %i to the constraining plane.\n",k);
	 gi.inputString(answer2,sPrintF(buff,"Enter tangent %i to the constraining plane (default=%8.2e,%8.2e,%8.2e)",
					k,v(0),v(1),v(2)));
	 if( answer2!="" )
	   sScanF(answer2,"%e %e %e",&v(0),&v(1),&v(2));

	 real norm = sqrt(sum(SQR(v)));
	 v/=max(REAL_MIN*100.,norm);
	 constraintValues(Range(9+3*k,11+3*k))=v;
      
	 printF(" Constraining plane: normalized tangent %i is (%7.4f,%7.4,%7.4f)\n",k,v(0),v(1),v(2));
       }
    }
    else if( answer=="rotation is constrained to a line" )
    {
       rotationConstraint=rotationConstrainedToALine;
      RealArray v(3); 
      v=0;  v(0)=1.;
      printF("Enter the tangent to the constraining line.\n");
      gi.inputString(answer2,sPrintF(buff,"Enter the tangent to the constraining line (default=%8.2e,%8.2e,%8.2e)",
				     v(0),v(1),v(2)));
      if( answer2!="" )
	sScanF(answer2,"%e %e %e",&v(0),&v(1),&v(2));

      real norm = sqrt(sum(SQR(v)));
      v/=max(REAL_MIN*100.,norm);
      constraintValues(Range(9,11))=v;
      
      printF(" Constraining line has a normalized tangent of (%7.4f,%7.4,%7.4f)\n",v(0),v(1),v(2));
    }
    else if( answer=="rotation is fixed" )
    {
       rotationConstraint=rotationIsFixed;
    }


//      else if( answer=="line constraint" )
//      {
//        constraint=pinnedToALine;

//        printF("Specify the tangent of the line through the center of mass along which the body is "
//               "constrained to move\n");
//        if( constraintValues.getLength(0)==0 )
//        {
//  	constraintValues.redim(3);
//          constraintValues=0.;
//  	constraintValues(0)=1.;
//        }
//        if( numberOfDimensions==2 ) constraintValues(2)=0.;
      
//        gi.inputString(answer2,sPrintF(buff,"Enter the tangent of the constraint line (default=%8.2e,%8.2e,%8.2e)",
//  				     constraintValues(0),constraintValues(1),constraintValues(2)));
//        if( answer2!="" )
//  	sScanF(answer2,"%e %e %e",&constraintValues(0),&constraintValues(1),&constraintValues(2));

//        // normalize the tangent
//        Range Rx=numberOfDimensions;
//        real norm = max( REAL_MIN*100., sqrt( sum(SQR(constraintValues(Rx))) ) );
//        constraintValues/=norm;

//        printF("The body will be constrained to move along the line with tangent (%8.2e,%8.2e,%8.2e)\n",
//             constraintValues(0),constraintValues(1),constraintValues(2));

//      }

    // -- new: commands from a dialog:
    else if( dialog.getToggleValue(answer,"relax correction steps",relaxCorrectionSteps) )
    {
      printF(" The rigid body correction steps can be relaxed. This is often necessary for 'light bodies'.\n"
             " There is a relaxation parameter for the force (alpha) and one for the torque (beta).\n"
             " Both alpha and beta should be a value in [0,1]. As a guess, choose\n"
             " alpha=.5*rho_f/M where rho_f is the density of the surrounding fluid and M is the mass\n"
             " of the rigid body. If the corrections don't converge, choose a smaller value of alpha.\n"
             " The value of beta is related to rho_f/M_I where M_I is a moment of inertial.\n"
             " The iteration for the force is said to converge when \n"
             "   (fDiff/fNorm)<fRelativeTol or fDiff < fAbsoluteTol , where fDiff=|f(new)-f(old)|\n"
             " A similar expression holds for the torque.\n"
	);
    }
    else if( dialog.getToggleValue(answer,"added mass",dbase.get<bool>("includeAddedMass")) )
    {
      if( dbase.get<bool>("includeAddedMass") )
      {
	printF("added mass:\n"
	       "  Added mass matrices will be included in the equations. These are used with light bodies\n");
        printF("  NOTE: I am also setting 'use extrapolation in predictor' and"
               " 'acceleration from differences' to true. You may turn these off if you want to.\n");

	useExtrapolationInPredictor=true;
        dialog.setToggleState("use extrapolation in predictor",useExtrapolationInPredictor);

	accelerationComputedByDifferencingVelocity=true;
        dialog.setToggleState("acceleration from differences",accelerationComputedByDifferencingVelocity);
      }
      
    }
    else if( dialog.getToggleValue(answer,"use extrapolation in predictor",useExtrapolationInPredictor) )
    {
      printF("use extrapolation in predictor:\n"
             "  For added mass cases we may want to extrapolate in time in the predictor instead\n"
             "  of using the equations of motion\n");
    }
    else if( dialog.getToggleValue(answer,"acceleration from differences",accelerationComputedByDifferencingVelocity) )
    {
      printF("acceleration from differences:\n"
	     "  For added mass cases we may want to return the acceleration (and angular acceleration) not computed\n"
             "  using the equations of motion but rather finite differences of the velocity (or angular velocity\n");
    }

    else if( dialog.getTextValue(answer,"debug:","%i",debug) ){}//
    else if( dialog.getTextValue(answer,"force relaxation parameter:","%g",correctionRelaxationParameterForce) ){}//
    else if( dialog.getTextValue(answer,"force relative tol:","%g",correctionRelativeToleranceForce) ){}//
    else if( dialog.getTextValue(answer,"force absolute tol:","%g",correctionAbsoluteToleranceForce) ){}//
    else if( dialog.getTextValue(answer,"torque relaxation parameter:","%g",correctionRelaxationParameterTorque) ){}//
    else if( dialog.getTextValue(answer,"torque relative tol:","%g",correctionRelativeToleranceTorque) ){}//
    else if( dialog.getTextValue(answer,"torque absolute tol:","%g",correctionAbsoluteToleranceTorque) ){}//

    else if( dialog.getTextValue(answer,"order of accuracy:","%i",dbase.get<int>("orderOfAccuracy")) ){} //
    else if( dialog.getTextValue(answer,"newtonTol:","%e",dbase.get<real>("toleranceNewton")) ){} //

    else if( dialog.getTextValue(answer,"predictor extrap order:","%i",orderOfExtrapolationPredictor) ){} //

    else if( dialog.getTextValue(answer,"log file:","%s",logFileName) )
    {
      if( logFile!=NULL )
      {
	fclose(logFile);
      }
      printF("Opening the rigid body log file = [%s]\n",(const char*)logFileName);
      logFile=fopen((const char*)logFileName,"w");
      fPrintF(logFile,"--- Rigid body log file ---\n");
      fPrintF(logFile," mass=%9.3e, (I1,I2,I3)=(%8.2e,%8.2e,%8.2e)\n",mass,mI(0),mI(1),mI(2));
    }
    

    else if( answer=="body force..." )
    {
      bodyForceOptionsDialog.showSibling();
    }
    else if( answer=="close bodyForce options" )
    {
      bodyForceOptionsDialog.hideSibling(); 
    } 
    else if( getBodyForceOption(answer,bodyForceOptionsDialog ) )
    {
      printF("Answer=%s found in getBodyForceOption\n",(const char*)answer);
    }
    else if( answer=="body force" ) // ** old way  **
    {
      printF("Specify a const body force to be applied to the rigid body at the center of mass.\n");
	gi.inputString(answer2,sPrintF(buff,"Enter const body force (default=%8.2e,%8.2e,%8.2e)",
				       bodyForceCoeff(0,0),bodyForceCoeff(1,0),bodyForceCoeff(2,0)));
	if( answer2!="" )
	  sScanF(answer2,"%e %e %e",&bodyForceCoeff(0,0),&bodyForceCoeff(1,0),&bodyForceCoeff(2,0));

    }

    else
    {
      printF("RigidBodyMotion::update:ERROR: unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
  }

  gi.unAppendTheDefaultPrompt();
  gi.popGUI(); // restore the previous GUI
  
  return 0;


}

