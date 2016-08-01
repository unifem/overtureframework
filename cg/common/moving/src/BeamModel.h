//                                   -*- c++ -*-
#ifndef BEAM_MODEL_H
#define BEAM_MODEL_H


#include "Overture.h"
#include "HyperbolicMapping.h"
#include "GenericGraphicsInterface.h"
#include "GridEvolution.h"
#include "DBase.hh"
using namespace DBase;

// Forward defs:
class GenericGraphicsInterface;
class MappingInformation;
class DeformingGrid;
class GridFunction; 
class Parameters;
class TravelingWaveFsi;

//................................
class BeamModel 
{
  static int globalBeamCounter; // keeps track of number of beams that have been created
 
 
protected: 
  // Longfei 20160127: make dbase protected to avoid changes from outside
  // Here is a database to hold parameters (new way)
  mutable DataBase dbase; 
  // type of the beam model
  aString beamType;


public:
  // Boundary conditions:
  // clamped: w=g, w_x=h
  // pinned:  w=g, w_xx=h   (simply supported)
  // freeBC:  EI*w_xx = M , EI*w_xxx = S , M=moment, S=transverse shear force
  // fourthBC:  w_x=g, EI*w_xxx = S  (this is possible but not implemented)
  // periodic: 
  //

  // Longfei 20160114:
  // do we need a parameter class to handle all the parameters???
  // do this for now ...
  enum beamModelTypeEnum
    {
      unknownBeamModel=-1,
      linearBeamModel=0,
      nonlinearBeamModel
    };

  enum spacialDiscretizationEnum
    {
      unknownSpacialDiscretization=-1,
      finiteElement=0,
      finiteDifference
    };

  enum TimeSteppingMethod
    {
      unknownTimeStepping=-1,
      //avaiblable predictor methods
      leapFrog=0,
      adamsBashforth2,
      newmark1,
      newmark2Explicit,
      newmark2Implicit,
      //avaiblable corrector methods
      newmarkCorrector=11,
      adamsMoultonCorrector=12,
    };

   
  enum BoundaryCondition
    { 
      unknownBC=-1, 
      pinned = 1 , 
      clamped = 2, 
      slideBC = 3,
      freeBC = 4 , 
      internalForceBC = 5, // used when computing the "internal force"  F = L(u,v) + f , given (u,v) 
      periodic = 8 
    };

  // constructor
  // sets the default parameters
  BeamModel();

  // destructor
  // 
  virtual ~BeamModel();

  // Longfei 20160115: applies to all derived beam models. 
  // make it non-virtual for now.
  // Add a constant body force to the beam.  This body
  // "force" has units of acceleration, e.g., g
  // i.e., it is not scaled by the density
  // bf: body force
  //
  void addBodyForce(const real bf[2]);

  // Longfei 20160115: Empty definition for base class. Each derived class implements its own version. Make it pure virtual??? This is an interface function called by DeformingBodyMotion
  // Add to the force on the beam 
  // virtual void addForce(const real & tf, const RealArray & x0, const RealArray & traction, const RealArray & normal,
  // 			const Index & Ib1, const Index & Ib2,  const Index & Ib3 )=0;

  //Longfei 20160621: new addForce function that add the surfaceForce to the current force
  //                           we now separate  setSurfaceForce and addForce
  virtual void addForce()=0;

  // ???Longfei 20160329: this function seems to be unused ... remove for now
  // Accumulate a pressure force to the beam from a fluid element.
  // virtual void addForce(const real & tf, const real& x0_1, const real& y0_1,
  // 			real p1, real p1x, const real& nx_1,const real& ny_1,
  // 			const real& x0_2, const real& y0_2,
  // 			real p2, real p2x, const real& nx_2,const real& ny_2);


  // add to the element integral for a function f
  void addToElementIntegral(const real & tf, const RealArray & x0, const RealArray & f, const RealArray & normal,  
                            const Index & Ib1, const Index & Ib2,  const Index & Ib3, RealArray & fe, 
                            bool addToForce= false );

  // add to the element integral for a function f
  void addToElementIntegral( const real & tf,
			     const real *x1, const real f1, const real f1x, const real *nv1, 
		  	     const real *x2, const real f2, const real f2x, const real *nv2,
			     RealArray & fe, bool addToForce = false );

  // Longfei 20160115: Empty definition for base class. Each derived class implements its own version. Make it pure virtual???
  // assign boundary conditions
  virtual int assignBoundaryConditions( real t, RealArray & x, RealArray & v, RealArray & a,const RealArray & f );

  // Longfei 20160115: applies to both FEM and FD models. 
  // assign initial conditions
  int assignInitialConditions( real t, RealArray & u, RealArray & v, RealArray & a );

  // Longfei 20160115: applies to all derived beam models. 
  // make it non-virtual for now.
  // choose an exact solution
  int chooseExactSolution(CompositeGrid & cg, GenericGraphicsInterface & gi );

  // Longfei 20160115: applies to all derived beam models. 
  // make it non-virtual for now.
  // choose initial conditions
  int chooseInitialConditions(CompositeGrid & cg, GenericGraphicsInterface & gi );

  // Apply the Newmark scheme corrector at t^{n+1}
  void corrector(real tnp1, real dt );

  // Longfei 20160118: make this pure virtual function 
  // Compute the acceleration of the beam.
  virtual void computeAcceleration(const real t,
                           const RealArray& u, const RealArray& v, 
  			   const RealArray& f,
  			   RealArray& a,
  			   real linAcceleration[2],
  			   real& omegadd,real dt,
                           const aString & solverName )=0;


  // Compute the integral of N(eta)*p from eta1 to eta2
  // this function converts the quantity (p1,p2) on beam surface to element integral on beam center line
  void computeProjectedForce(real p1, real p2, 
			     real eta1, real eta2,
			     RealArray& fe);
  
  // compute local element Galerkin projection
  void computeGalerkinProjection(real fa, real fap, real fb, real fbp, 
				 real a, real b,
				 RealArray & f );


  //Longfei 20160622: 
  // new function handles debug. New way to handle debug. it is no longer a static member
  const int & debug() const { return dbase.get<int>("debug");}



  // Return the current displacement of the structure.  (Displacement solution u[current])
  // Longfei 20160120: renamed this function position() --> displacement()
  // since it returns the displacement on DOF not the position
  const RealArray& displacement() const;

  // Write dbase keys and datatype:
  void displayDBase(FILE *file=stdout);
  
  // Longfei 20160115: applies to all derived beam models. 
  // make it non-virtual for now.
  // Return the exact velocity of the FLUID for the FSI analytical solution
  // derived in the documentation
  static void exactSolutionVelocity(real x, real y,real t,
				    real k, real H, 
				    real omega_real, real omega_imag,
				    real omega0, real nu,
				    real what,
				    real& u, real& v);

  // Longfei 20160115: applies to all derived beam models. 
  // make it non-virtual for now.
  // Return the exact pressure of the FLUID for the FSI analytical solution
  // derived in the documentation
  static void exactSolutionPressure(real x, real y,real t,
				    real k, real H, 
				    real omega_real, real omega_imag,
				    real omega0, real nu,
				    real what,
				    real& p);

  //Longfei 20160206: new function to factor block  tridiangnal solvers
  // for FEMBeamModel and/or projection between beam surface and beam neutal line
  int factorBlockTridiagonalSolver(const aString & tridiagonalSolverName);


  // convert BoundaryCondition to its name in a string
  aString getBCName(const BoundaryCondition & bc) const;

  // Longfei 20160115: applies to all derived beam models. 
  // make it non-virtual for now.
  // Return the current force of the structure.
  //renamed force() to getCurrentForce()
  const RealArray& getCurrentForce() const;

    // convert TimeSteppingMethod to its name in a string
  aString getTSName(const TimeSteppingMethod & ts) const;

  // for public access of beamID (read only)
  // Return the beam ID (a unique ID for this beam)
  const int& getBeamID() const; 

  //Longfei 20160622:
  const aString& getBeamType() const;

  // Longfei 20160117: applies to all derived beam models. 
  // compute an exact eigenmode solution 
  int getBeamEigenmode( real t, RealArray & u, RealArray & v, RealArray & a ) const;

  // Longfei 20160117: applies to all derived beam models. 
  // compute the beam-piston exact solution
  int getBeamPiston( real t, RealArray & u, RealArray & v, RealArray & a ) const;


  //???Longfei 20160117: not sure what this does. come back later ...
  // Get beam reference coordinates and direction array (indicates which side of the beam)
  int getBeamReferenceCoordinates( const RealArray & x0, RealArray & s0, IntegerArray & elementNumber,
				   RealArray & signedDistance );

 
  // Longfei 20160117: this should apply to all beam models.
  // compute the beam-under-pressure exact solution
  int getBeamUnderPressure( real t, RealArray & u, RealArray & v, RealArray & a ) const;


  // Longfei 20160117: applies to all derived beam models. 
  // make it non-virtual for now. Make sure the index work for FD Model as well FIX ME...
  // Return the (x,y) coordinates of the centerline
  //
  void getCenterLine( RealArray & xc, bool scaleDisplacementForPlotting=false ) const;

  // Longfei 20160127: get current time
  real getCurrentTime() const;


  // Longfei 20160117: applies to all derived beam models. 
  // make it non-virtual for now.
  // return the estimated *explicit* time step dt 
  real getExplicitTimeStep() const;


  // Longfei 20160122:
  // for public access of exactSolutionOption (read only)
  aString getExactSolutionOption() const;

  // Longfei 20160117: this should apply to all beam models. 
  // Compute errors in the solution (when the solution is known)
  int getErrors( const real t, const RealArray & u, const RealArray & v, const RealArray & a,
			 const aString & label, FILE *file = stdout, real *uvErr=NULL, real *uvNorm=NULL );

  // Longfei 20160117: applies to all derived beam models. 
  // make it non-virtual for now.
  // Compute errors in the current solution.
  int getErrors( const aString & label,
		 FILE *file=stdout,
		 real *uvErr= NULL, real *uvNorm= NULL );

  // Longfei 20160117: this should apply to all beam models.
  // Return the exact solution (if any)
  int getExactSolution( real t, RealArray & u, RealArray & v, RealArray & a ) const;



  // Longfei 20160120: made this function virtual
  // Return nodal force values on beam center-line
  virtual void getForceOnBeam( const real t, RealArray & force )=0;

  // Longfei 20160120: this should apply to all beam models
  // Get the beam's mass per unit length ( rho*A = rho*h*b in  2D)
  int getMassPerUnitLength( real & rhoA ) const;

  // Longfei 20160120: this should apply to all beam models
  // return maximum relative correction for sub-iterations
  real getMaximumRelativeCorrection() const;



  // Longfei 20160120: this should apply to all beam models
  // return the number of elements
  int getNumberOfElements() const; // 

  // Longfei 20160120: this should apply to all beam models
  // Obtain a past time solution (e.g. needed by deforming grids)
  int getPastTimeState( const real pastTime, RealArray & xPast, const real t0, const RealArray x0 );


  // return the value of an integer  parameter
  int getParameter( const aString & name, int & value ) const;

  // return the value of a real  parameter
  int getParameter( const aString & name, real & value ) const;

  // Longfei 20160117: this should apply to all beam models. modifications are made for FDBeamModel
  // evaluate the standing wave solution.
  int getStandingWave( real t, RealArray & u, RealArray & v, RealArray & a ) const;

  // Get points on the beam surface
  void getSurface( const real t, const RealArray & x0,  const RealArray & xs, 
                   const Index & Ib1, const Index & Ib2,  const Index & Ib3,
                   const bool adjustEnds = false );


  // return an estimate of the time-step dt
  real getTimeStep() const;

  // Longfei 20160117: this should apply to all beam models. modifications are made for FDBeamModel
  // evaluate the FSI traveling wave solution
  int getTravelingWaveFSI( real t, RealArray & u, RealArray & v, RealArray & a ) const;

  // Longfei 20160117:new function applies to all derived beam models. 
  // compute an twilightZone solution (u,v,a) at t
  int getTwilightZone( real t, RealArray & u, RealArray & v, RealArray & a ) const;

  // Longfei 20160211: new function to define the data structre for FEM and FD model
  int getSolutionArrayIndex(Index & I1, Index &I2, Index & I3, Index &C) const;

  // Get the acceleration of points on the beam surface
  void getSurfaceAcceleration( const real t, const RealArray & x0, RealArray & as, const RealArray & normal, 
                               const Index & Ib1, const Index & Ib2,  const Index & Ib3,
                               const bool adjustEnds = false );

  // Get the velocity of points on the beam surface
  void getSurfaceVelocity( const real t, const RealArray & x0,  const RealArray & vs, 
                           const Index & Ib1, const Index & Ib2,  const Index & Ib3,
                           const bool adjustEnds = false );


  // Get the 'internal force' on the beam surface (used by added mass algorithms)
  void getSurfaceInternalForce( const real t, const RealArray & x0, RealArray & fs, 
				const RealArray & normal, 
				const Index & Ib1, const Index & Ib2,  const Index & Ib3,
				const bool addExternalForcing );

  // Returns true if the fixed point iteration to alleviate
  // the added mass effect has converged.
  //
  bool hasCorrectionConverged() const;

  // Does the beam on fluid on both sides.
  bool hasFluidOnTwoSides() const;

  // invert a 2x2 matrix
  static void inverse2x2(const RealArray& A, RealArray& inv); 

  // output probe info
  int outputProbes( real t, int stepNumber );


  // Longfei 20160120: needs to modify this function to work with both FD and FEM data structures; done!
  // plot the solution and errors
  int plot( real t, GenericGraphicsInterface & gi, GraphicsParameters & psp , const aString & label );

  // print time step info
  void printTimeStepInfo( FILE *file=stdout );

  // Return the displacement of the point on the surface (not the neutral axis)
  // of the beam of the point whose undeformed location is (x0,y0).
  void projectDisplacement(const real t, const RealArray& X, const real& x0, const real& y0, real& x, real& y,
			   bool clipToBounds=true   );

  // Return the acceleration of the point on the surface (not the neutral axis)
  // of the beam of the point whose undeformed location is (x0,y0).
  void projectAcceleration(const real t, const real& x0, const real& y0, real& ax, real& ay);

  // Return the "interal force" of a point on the surface of the beam of the point whose undeformed location is (x0,y0).
  void projectInternalForce( const RealArray & internalForce, const real t, const real& x0, const real& y0, real& ax, real& ay);

  // Project the current surface velocity onto the beam (and over-write current beam velocity)
  void projectSurfaceVelocityOntoBeam( const real t );

  // Return the velocity of the point on the surface (not the neutral axis)
  void projectVelocity( const real t, const real& x0, const real& y0, real& vx, real& vy );


  // Predict the structural state at t^{n+1} = t + dt, using the Newmark beta predictor.
  void predictor(real tnp1, real dt );


  //Longfei 20160117: new function added to set the dimension of solution array.
  int redimSolutionArray(RealArray & u, RealArray & v, RealArray & a) const;

  
  // Set the current force on the beam to zero.  Typical usage is to call
  // resetForce() to zero the force, then call addForce() for every fluid
  // element to accumulate the pressure load, then call predictor()/corrector()
  // to recompute the acceleration.
  //
  void resetForce();

  // Set the surface velocity to zero. (used to project the velocity) 
  void resetSurfaceVelocity();

  // Set the relaxation parameter used in the fixed point iteration
  // used to alleviate the added mass effect
  // omega: relaxation factor (default is 1.0)
  //
  void setAddedMassRelaxation(double omega);

  // Longfei 20160503: this function is removed.
  // parameters can only be changed via update() function.
  // Set a real beam parameter (in the dbase). 
  //int setParameter( const aString & name, real & value );

  // Longfei 20160330: new function that sets the surface force:
  // Set the surface force (used to project traction on beam surface to beam neutral line)
  // results are load vector of the force(traction) on beam neutral line
  // Here traction = -sigma*n on beam surface with indices Ib1,Ib2,Ib3
  void setSurfaceForce(const real & t, const RealArray & x0, const RealArray & traction, 
				  const RealArray & normal, const Index & Ib1, const Index & Ib2,  const Index & Ib3 );

  // Set the surface velocity (used to project the beam velocity)
  // results are load vector of the velocity on beam neutral line
  void setSurfaceVelocity(const real & t, const RealArray & x0, const RealArray & vSurface, 
				  const RealArray & normal, const Index & Ib1, const Index & Ib2,  const Index & Ib3 );



  // Allow the beam to undergo free motion.  
  // x0:    initial center of mass of the beam (x)
  // y0:    initial center of mass of the beam (y)
  // angle: initial angle of the beam
  //
  void setupFreeMotion(real x0,real y0, real angle0);

  // Set the initial angle of the beam, from the x axis (in radians)
  void setDeclination(real dec);

  // Longfei 20160121: this function is no longer needed. Removed
  // Provide the TravelingWaveFsi object that defines an exact solution.
  // int setTravelingWaveSolution( TravelingWaveFsi & tw );

  //  Solve A*u = f 
  // old:
  // void solveBlockTridiagonal(const RealArray& Ae, const RealArray& f, RealArray& u, 
  //                            const real alpha, const real alphaB, 
  //                            const aString & tridiagonalSolverName );
  // Longfei 20160208: new interface
  void solveBlockTridiagonal(const RealArray& f, RealArray& u, const aString & tridiagonalSolverName );
  
  // Return the "surfaceVelocity" array (used for projecting the beam velocity in FSI simulations)
  const RealArray& surfaceVelocity() const;

  // Set parameters interactively: 
  int update(CompositeGrid & cg, GenericGraphicsInterface & gi );

  // Return the current velocity DOF's of the structure.
  const RealArray& velocity() const;

  // Write information to the `check file' 

  int writeCheckFile( real t,FILE *file );

  // Write information about the beam
  void writeParameterSummary( FILE *file= stdout );




  // allow TestBeamModel to access dbase
  friend class TestBeamModel; 

  // Longfei 20160115: make everything public for now... 
  //  ---------------------------------- PRIVATE ---------------------------------------------------------
  //private:

  // Longfei 20160121: make this function pure virtual.
  // add internalforces such as buoyance and TZ forcing
  // the base version gets the nodal values of the internal forces; for FEM beam, nodal values of x derivatives are computed as well
  // the derived versions add the nodal values to the forcing f in appropriate data structure
  virtual void addInternalForces( const real t, RealArray & f )=0;



  // Compute the internal force in the beam
  virtual void computeInternalForce( const RealArray& u, const RealArray& v, RealArray& f )=0;





  //  Return the RHS values for the boundary conditions.
  int getBoundaryValues( const real t, RealArray & g, const int ntd=0 );

  // Longfei 2016038: made change to this function so that it works for FDBeamModel as well. 
  // The interpolation for FEM and FD models are different
  // Compute the slope and displacement of the beam at a given
  // element # and coordinate
  // X:            Beam solution (position)
  // elemNum:      element number on which the solution is desired
  // eta:          element natural coordinate where the solution is desired
  // displacement: displacement at this point [out]
  // slope:        slope at this point [out]
  //
  void interpolateSolution(const RealArray& X,
				   int& elemNum, real& eta,
				   real& displacement, real& slope);

  // initialize TZ
  int initTwilightZone();

  // -- initialize the beam model given the current parameters --
  // each derived class calls the base initialize() and do more initializations as needed
  virtual void initialize();


  // Longfei 20160731: this functions looks like unused. Removed
  // Compute the third derivative, w'''(x), of the beam displacement w(x) at a given
  // void interpolateThirdDerivative(const RealArray& X,
  // 				  int& elemNum, real& eta,
  // 				  real& deriv3);



  real norm( RealArray & u ) const;

  // Return the element, thickness, and natural coordinate for
  // a point (x0,y0) on the undeformed SURFACE of the beam
  void projectPoint(const real& x0,const real& y0,
		    int& elemNum, real& eta, real& halfThickness,
                    bool clipToBounds=true);

  
  // For internal use with free motion.  Recomputes the normal and tangent vectors
  // for the beam (based on the current angle)
  //
  void recomputeNormalAndTangent();

  // smooth the solution: Longfei 20160302: made this pure virtual
  virtual void smooth( const real t, RealArray & w, const aString & label )=0;

  // Longfei 20160302: moved this to FEMBeamModel. FDBeamModel does not need this.
  // We apply actual bc for FDBeamModel smoother
  // assign boundary conditions for the smooth function
  // int smoothBoundaryConditions( RealArray & w1, int base, int bound, int numberOfGhost, int orderOfExtrapolation );



 



  // Longfei 20160122: leave these here for now...
  // will come back if we want to work on free motion
  // ------------------------------------------------------------
  // The following parameters are used exclusively in the case of 
  // free motion
  // ------------------------------------------------------------

  // CoM values for free motion
  //
  real centerOfMass[2], centerOfMassVelocity[2], centerOfMassAcceleration[2];

  // predictor values for the CoM parameters in free motion
  //
  real comVtilde[2],comXtilde[2];
  
  // angular values for free motion
  //
  real angle, angularVelocity, angularAcceleration;
  
  // angular values (predictors) for free motion
  real angletilde,angularVelocityTilde;

  // current normal/tangent.
  real normal[2],tangent[2];

  // Total force on the beam (the integral of p*n over the surface)
  //
  real totalPressureForce;
  
  // Total moment on the beam (the integral of x*p*n over the surface)
  //
  real totalPressureMoment;

  // Total inertia and mass of the beam
  // real totalInertia, totalMass;

  // Acceleration in the free motion case at the previous fixed 
  // point iteration
  real old_rb_acceleration[3];

  // Penalty parameter (for enforcing pinned boundary conditions)
  //
  real penalty;

  // Initial location of the left end of the beam
  //
  real initialEndLeft[2];
  
  // Initial location of the right end of the beam
  //
  real initialEndRight[2];



};

#endif




// ========= removed stuff =================

  // Longfei 20160115: FEM specific function. Moved to FEMBeamModel
  // add to the element integral for a function f
  // void addToElementIntegral(const real & tf, const RealArray & x0, const RealArray & f, const RealArray & normal,  
  //                           const Index & Ib1, const Index & Ib2,  const Index & Ib3, RealArray & fe, 
  //                           bool addToForce= false );

  // Longfei 20160115: FEM specific function. Moved to FEMBeamModel  
  // add to the element integral for a function f
  // void addToElementIntegral( const real & tf,
  //			     const real *x1, const real f1, const real f1x, const real *nv1, 
  //		  	     const real *x2, const real f2, const real f2x, const real *nv2,
  //			     RealArray & fe, bool addToForce = false );

  // Longfei 20160120: this seems unused. Removed
  // Get the exact pressure to be applied to the beam
  // from the analytical solution derived in the documentation
  // t: Time at which to compute the exact pressure
  // x: Location on the beam to get the pressure
  //
  // double getExactPressure(double t, double x);


  // Longfei 20160120: this should apply to all beam models
  // Return the name of this beam
  //const aString & getName() const { return name; } // 


  // Longfei 20160120: this function seems unused. Removed
  // This function initializes the beam model.
  // void setParameters(real momOfIntertia, real E, 
  // 		     real rho,real beamLength,
  // 		     real thickness,real pnorm,
  // 		     int nElem,BoundaryCondition bcleft,
  // 		     BoundaryCondition bcright, 
  // 		     real x0, real y0,
  // 		     bool useExactSolution);


  // Longfei 20160120: oldTravelingWaveFsi removed
  // Get the exact position, velocity, and acceleration of the beam
  // from the analytical solution derived in the documentation
  // t: Time at which to compute the exact solution
  // x: Position of the beam
  // v: Velocity of the beam
  // a: Acceleration of the beam
  //
  //void setExactSolution(double t,RealArray& x, RealArray& v, RealArray& a) const;

  
  // Longfei 20160120: this function seems unused. Remove
  // Set the (relative) convergence tolerance for the fixed point iteration
  // tol: convergence tolerance (default is 1.0e-3)
  //
  //void setSubIterationConvergenceTolerance(double tol);


  // Longfei 20160115:  Removed since *THIS FUNCTION NOT USED ANYMORE*
  // set the surface velocity over an interval
  // virtual void setSurfaceVelocity(const real & tf, const real& x0_1, const real& y0_1,
  // 			  real v1,  real v1x, const real& nx_1,const real& ny_1,
  // 			  const real& x0_2, const real& y0_2,
  // 			  real v2, real v2x, const real& nx_2,const real& ny_2);



  // Longfei 20160121: moved to FEMBeamModel
  // compute local element Galerkin projection
  // void computeGalerkinProjection(real fa, real fap, real fb, real fbp, 
  // 			       real a, real b,
  // 			       RealArray & f );



  //  Longfei 20160118: moved to FEMBeamModel
  // Multiply a vector w by the mass matrix
  //void multiplyByMassMatrix(const RealArray& w, RealArray& Mw);


 // Longfei 20160120: parameters moved into dbase
  //int domainDimension;  // domain dimension
  //int rangeDimension;   // number of space dimensions (range)

  // This is actually I/b, that is the true area moment of inertia
  // divided by the width of the beam
  //
  // real areaMomentOfInertia;

  // Beam elastic modulus
  //
  // real elasticModulus;

  // Beam density
  //
  //real density;

  // Beam thickness and breadth
  //real thickness, breadth;

  // Beam mass per unit length (actually mass per unit length / b)
  //
  //real massPerUnitLength;

  // Same as massPerUnitLength, except that the density used to compute
  // this value is (beamDensity - fluidDensity)
  //
  //real buoyantMassPerUnitLength;

  // fluidDensity*beamVolume
  //
  //real buoyantMass;

  // value used to try to enforce the cantilever condition in free motion
  // doesn't really work
  //
  // real leftCantileverMoment; //Longfei 20160121: moved to FEMBeamModel

  // Total beam length
  //
  // real L;

  // Element length
  //
  //real le;

  // Left end of the beam (undeformed),
  // and the initial angle of the beam
  //
  // real beamX0, beamY0, beamZ0;  // put in dbase as  real beamXYZ[3]
  // real beamInitialAngle; //Longfei 20160121: we might need more beamInitialAngle for 3D problem

  // Initial beam normal and tangent vectors.
  //
  //real initialBeamNormal[2],initialBeamTangent[2];


  // Longfei 20160118: moved to FEMBeamModel
  // Element stiffness and mass matrices.  
  // Note that in this model they are constant in time
  // and the same for every element
  //
  // RealArray elementK, elementM;

  // Total number of elements
  // int numElem;
  
  // Current time step.  Incremented on a call to predictor()
  //
  //int numberOfTimeSteps;

  //RealArray aold; // holds old acceleration for under-relaxed iteration


 // Predicted position/velocity of the beam
  //
  // RealArray dtilde,vtilde;

  // Parameters for the newmark beta scheme 
  //
  //real newmarkBeta, newmarkGamma;

  // Value used to scale the pressure (e.g., the fluid density)
  //
  //real pressureNorm;

  // At the first time step, no acceleration typically exists,
  // so it must be computed from the force
  // 
  //bool hasAcceleration;
   
  // True if the fixed point iteration to alleviate
  // the added mass effect has converged.
  //
  // bool correctionHasConverged;

  // Boundary conditions on the left and right of the beam.  see above
  //
  //BoundaryCondition bcLeft, bcRight;

  // True if the simulation is being done using the exact analytical
  // solution from the documentation
  // bool useExactSolution;

  // Initial residual of the fixed point iteration.  Converegence is 
  // reached when residual < tol*initialResidual
  //
  // double initialResidual;

  // Number of corrector iterations that have been taken
  //
  // int numCorrectorIterations;

  // Body force (not including the density on the beam).  
  // typically set to be the gravitational acceleration
  //
  // real bodyForce[2];

  // Body force component in the direction of the normal of the beam
  //
  // real projectedBodyForce;

  // True if the beam is allowed to float free
  //
  //bool allowsFreeMotion;


  // exact solution option
  //aString exactSolutionOption;

  // name for the initial conditions
  // aString initialConditionOption;


  
  //  Unique ID for this beam
  //  int beamID;
