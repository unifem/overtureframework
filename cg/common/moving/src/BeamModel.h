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

 public:

  // Boundary conditions:
  // clamped: w=g, w_x=h
  // pinned:  w=g, w_xx=h   (simply supported)
  // freeBC:  EI*w_xx = M , EI*w_xxx = S , M=moment, S=transverse shear force
  // fourthBC:  w_x=g, EI*w_xxx = S  (this is possible but not implemented)
  // periodic: 
  //
  enum BoundaryCondition
  { 
    unknownBC=-1, 
    pinned = 1 , 
    clamped = 2, 
    freeBC = 4 , 
    internalForceBC = 5, // used when computing the "internal force"  F = L(u,v) + f , given (u,v) 
    periodic = 8 
  };

  // constructor
  // sets the default parameters
  BeamModel();

  // destructor
  // 
  ~BeamModel();

  // Add a constant body force to the beam.  This body
  // "force" has units of acceleration, e.g., g
  // i.e., it is not scaled by the density
  // bf: body force
  //
  void addBodyForce(const real bf[2]);

  // Add to the force on the beam
  void addForce(const real & tf, const RealArray & x0, const RealArray & traction, const RealArray & normal,
		const Index & Ib1, const Index & Ib2,  const Index & Ib3 );

  // Accumulate a pressure force to the beam from a fluid element.
  void addForce(const real & tf, const real& x0_1, const real& y0_1,
		real p1, real p1x, const real& nx_1,const real& ny_1,
		const real& x0_2, const real& y0_2,
		real p2, real p2x, const real& nx_2,const real& ny_2);

// add to the element integral for a function f
void addToElementIntegral(const real & tf, const RealArray & x0, const RealArray & f, const RealArray & normal,  
                     const Index & Ib1, const Index & Ib2,  const Index & Ib3, RealArray & fe, 
                     bool addToForce= false );

// add to the element integral for a function f
void addToElementIntegral( const real & tf,
			   const real *x1, const real f1, const real f1x, const real *nv1, 
			   const real *x2, const real f2, const real f2x, const real *nv2,
			   RealArray & fe, bool addToForce = false );

  // assign boundary conditions
  int assignBoundaryConditions( real t, RealArray & x, RealArray & v, RealArray & a );

  // assign initial conditions
  int assignInitialConditions( real t, RealArray & x, RealArray & v, RealArray & a );

  // choose an exact solution
  int chooseExactSolution(CompositeGrid & cg, GenericGraphicsInterface & gi );

  // choose initial conditions
  int chooseInitialConditions(CompositeGrid & cg, GenericGraphicsInterface & gi );

  // Apply the Newmark scheme corrector at t^{n+1}
  void corrector(real tnp1, real dt );

  // Return the exact velocity of the FLUID for the FSI analytical solution
  // derived in the documentation
  static void exactSolutionVelocity(real x, real y,real t,
				    real k, real H, 
				    real omega_real, real omega_imag,
				    real omega0, real nu,
				    real what,
				    real& u, real& v);

  // Return the exact pressure of the FLUID for the FSI analytical solution
  // derived in the documentation
  static void exactSolutionPressure(real x, real y,real t,
				    real k, real H, 
				    real omega_real, real omega_imag,
				    real omega0, real nu,
				    real what,
				    real& p);

  // Return the current force of the structure.
  //
  const RealArray& force() const;

  // Return the beam ID (a unique ID for this beam)
  int getBeamID() const{ return beamID; } // 

  // compute the beam-piston exact solution
  int getBeamPiston( real t, RealArray & u, RealArray & v, RealArray & a ) const;

  // Get beam reference coordinates and direction array (indicates which side of the beam)
  int getBeamReferenceCoordinates( const RealArray & x0, RealArray & s0, IntegerArray & elementNumber,
				   RealArray & signedDistance );

  // compute the beam-under-pressure exact solution
  int getBeamUnderPressure( real t, RealArray & u, RealArray & v, RealArray & a ) const;

  // Return the (x,y) coordinates of the centerline
  //
  void getCenterLine( RealArray & xc, bool scaleDisplacementForPlotting=false ) const;

  // return the estimated *explicit* time step dt 
  real getExplicitTimeStep() const;

  // Compute errors in the solution (when the solution is known)
  int getErrors( const real t, const RealArray & u, const RealArray & v, const RealArray & a,
                 const aString & label, FILE *file = stdout );

  // Return the exact solution (if any)
  int getExactSolution( real t, RealArray & u, RealArray & v, RealArray & a ) const;

  // Get the exact pressure to be applied to the beam
  // from the analytical solution derived in the documentation
  // t: Time at which to compute the exact pressure
  // x: Location on the beam to get the pressure
  //
  double getExactPressure(double t, double x);

  // Return nodal force values on beam center-line
  void getForceOnBeam( const real t, RealArray & force );


  // Get the beam's mass per unit length ( rho*A = rho*h*b in  2D)
  int getMassPerUnitLength( real & rhoA ) const;

  // return maximum relative correction for sub-iterations
  real getMaximumRelativeCorrection() const;

  // Return the name of this beam
  const aString & getName() const { return name; } // 

  // return the number of elements
  int getNumberOfElements() const { return numElem; } // 

  // Obtain a past time solution (e.g. needed by deforming grids)
  int getPastTimeState( const real pastTime, RealArray & xPast, const real t0, const RealArray x0 );

  // return the value of a integer  parameter
  int getParameter( const aString & name, int & value ) const;

  // return the value of a real  parameter
  int getParameter( const aString & name, real & value ) const;

  // evaluate the standing wave solution
  int getStandingWave( real t, RealArray & u, RealArray & v, RealArray & a ) const;

  // Get points on the beam surface
  void getSurface( const real t, const RealArray & x0,  const RealArray & xs, 
                   const Index & Ib1, const Index & Ib2,  const Index & Ib3,
                   const bool adjustEnds = false );

  // Return the "surfaceVelocity" array (used for projecting the beam velocity in FSI simulations)
  const RealArray& getSurfaceVelocity() const;

  // return an estimate of the time-step dt
  real getTimeStep() const;

  // evaluate the FSI traveling wave solution
  int getTravelingWaveFSI( real t, RealArray & u, RealArray & v, RealArray & a ) const;

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

  // Return the current position of the structure.
  //
  const RealArray& position() const;


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

  // Set a real beam parameter (in the dbase). 
  int setParameter( const aString & name, real & value );

  // This function initializes the beam model.
  void setParameters(real momOfIntertia, real E, 
		     real rho,real beamLength,
		     real thickness,real pnorm,
		     int nElem,BoundaryCondition bcleft,
		     BoundaryCondition bcright, 
		     real x0, real y0,
		     bool useExactSolution);


  // Get the exact position, velocity, and acceleration of the beam
  // from the analytical solution derived in the documentation
  // t: Time at which to compute the exact solution
  // x: Position of the beam
  // v: Velocity of the beam
  // a: Acceleration of the beam
  //
  void setExactSolution(double t,RealArray& x, RealArray& v, RealArray& a) const;

  
  // Set the (relative) convergence tolerance for the fixed point iteration
  // tol: convergence tolerance (default is 1.0e-3)
  //
  void setSubIterationConvergenceTolerance(double tol);

  // Set the surface velocity (used to project the beam velocity)
  void setSurfaceVelocity(const real & t, const RealArray & x0, const RealArray & vSurface, 
                          const RealArray & normal, const Index & Ib1, const Index & Ib2,  const Index & Ib3 );

  // set the surface velocity over an interval
  void setSurfaceVelocity(const real & tf, const real& x0_1, const real& y0_1,
			  real v1,  real v1x, const real& nx_1,const real& ny_1,
			  const real& x0_2, const real& y0_2,
			  real v2, real v2x, const real& nx_2,const real& ny_2);


  // Allow the beam to undergo free motion.  
  // x0:    initial center of mass of the beam (x)
  // y0:    initial center of mass of the beam (y)
  // angle: initial angle of the beam
  //
  void setupFreeMotion(real x0,real y0, real angle0);

  // Set the initial angle of the beam, from the x axis (in radians)
  void setDeclination(real dec);

  // Provide the TravelingWaveFsi object that defines an exact solution.
  int setTravelingWaveSolution( TravelingWaveFsi & tw );

  // Set parameters interactively: 
  int update(CompositeGrid & cg, GenericGraphicsInterface & gi );

  // Return the current velocity DOF's of the structure.
  const RealArray& velocity() const;

  // Write information about the beam
  void writeParameterSummary( FILE *file= stdout );

  /// Here is a database to hold parameters (new way)
  mutable DataBase dbase; 

  static real exactSolutionScaleFactorFSI;  // scale factor for the exact FSI solution 
  static int debug;

  friend class TestBeamModel;

 //  ---------------------------------- PRIVATE ---------------------------------------------------------
 private:

  // add internalforces such as buoyance and TZ forcing 
  int addInternalForces( const real t, RealArray & f );

  // Compute the acceleration of the beam.
  void computeAcceleration(const real t,
                           const RealArray& u, const RealArray& v, 
			   const RealArray& f,
			   const RealArray& A,
			   RealArray& a,
			   real linAcceleration[2],
			   real& omegadd,real dt,
                           const real alpha, const real alphaB, 
                           const aString & tridiagonalSolverName );

  // compute local element Galerkin projection
  void computeGalerkinProjection(real fa, real fap, real fb, real fbp, 
			       real a, real b,
			       RealArray & f );

  // Compute the internal force in the beam, i.e., -B*v -K*u
  void computeInternalForce( const RealArray& u, const RealArray& v, RealArray& f );


  // Compute the integral of N(eta)*p, that is, the rhs of the FEM model, for a particular element
  void computeProjectedForce(real p1, real p2, 
			     real eta1, real eta2,
			     RealArray& fe);


  //  Return the RHS values for the boundary conditions.
  int getBoundaryValues( const real t, RealArray & g, const int ntd=0 );

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
  void initialize();

  // Compute the third derivative, w'''(x), of the beam displacement w(x) at a given
  void interpolateThirdDerivative(const RealArray& X,
				  int& elemNum, real& eta,
				  real& deriv3);

  // Multiply a vector w by the mass matrix
  void multiplyByMassMatrix(const RealArray& w, RealArray& Mw);

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

  // smooth the solution
  void smooth( const real t, RealArray & w, const aString & label );

  //  Solve A*u = f 
  void solveBlockTridiagonal(const RealArray& Ae, const RealArray& f, RealArray& u, 
                             const real alpha, const real alphaB, 
                             const aString & tridiagonalSolverName );

  int domainDimension;     // domain dimension
  int numberOfDimensions;  // number of space dimensions (range)

  // This is actually I/b, that is the true area moment of inertia
  // divided by the width of the beam
  //
  real areaMomentOfInertia;

  // Beam elastic modulus
  //
  real elasticModulus;

  // Beam density
  //
  real density;

  // Beam thickness and breadth
  real thickness, breadth;

  // Beam mass per unit length (actually mass per unit length / b)
  //
  real massPerUnitLength;

  // Same as massPerUnitLength, except that the density used to compute
  // this value is (beamDensity - fluidDensity)
  //
  real buoyantMassPerUnitLength;

  // fluidDensity*beamVolume
  //
  real buoyantMass;

  // value used to try to enforce the cantilever condition in free motion
  // doesn't really work
  //
  real leftCantileverMoment;

  // Total beam length
  //
  real L;

  // Element length
  //
  real le;

  // Left end of the beam (undeformed),
  // and the initial angle of the beam
  //
  real beamX0, beamY0, beamZ0, beamInitialAngle;

  // Initial beam normal and tangent vectors.
  //
  real initialBeamNormal[2],initialBeamTangent[2];

  // Element stiffness and mass matrices.  
  // Note that in this model they are constant in time
  // and the same for every element
  //
  RealArray elementK, elementM;

  // Total number of elements
  //
  int numElem;

  // Current time step.  Incremented on a call to predictor()
  //
  int numberOfTimeSteps;

  RealArray aold; // holds old acceleration for under-relaxed iteration

  // Current time
  //
  real t;

  // Predicted position/velocity of the beam
  //
  RealArray dtilde,vtilde;

  // Parameters for the newmark beta scheme 
  //
  real newmarkBeta, newmarkGamma;

  // Value used to scale the pressure (e.g., the fluid density)
  //
  real pressureNorm;

  // At the first time step, no acceleration typically exists,
  // so it must be computed from the force
  // 
  bool hasAcceleration;
   
  // True if the fixed point iteration to alleviate
  // the added mass effect has converged.
  //
  bool correctionHasConverged;

  // Boundary conditions on the left and right of the beam.  see above
  //
  BoundaryCondition bcLeft, bcRight;

  // True if the simulation is being done using the exact analytical
  // solution from the documentation
  bool useExactSolution;

  // Initial residual of the fixed point iteration.  Converegence is 
  // reached when residual < tol*initialResidual
  //
  double initialResidual;

  // Number of corrector iterations that have been taken
  //
  int numCorrectorIterations;

  // Body force (not including the density on the beam).  
  // typically set to be the gravitational acceleration
  //
  real bodyForce[2];

  // Body force component in the direction of the normal of the beam
  //
  real projectedBodyForce;

  // True if the beam is allowed to float free
  //
  bool allowsFreeMotion;

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
  real totalInertia, totalMass;

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


  // exact solution option
  aString exactSolutionOption;

  // name for the initial conditions
  aString initialConditionOption;

  // Name for the beam
  aString name;

  //  Unique ID for this beam
  int beamID;
  static int globalBeamCounter; // keeps track of number of beams that have been created

};

#endif
