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

//................................
class BeamModel {

 public:

  // Pinned:      x,y are fixed, theta is free
  // Cantilever:  x,y,theta are fixed
  // Free:        x,y,theta are all free to move
  // XXX Periodic is unimplemented!
  //
  enum BoundaryCondition { Pinned = 1 , Cantilevered = 2, Free = 4 , Periodic = 8 };

  // constructor
  // sets the default parameters
  BeamModel();

  // destructor
  // 
  ~BeamModel();

  // This function initializes the beam model.
  // momOfIntertia:    I/b (true area moment of inertia divided by the width of the beam
  // E:                Elastic modulus
  // rho:              beam density
  // thickness:        beam thickness (assumed to be constant)
  // pnorm:            value used to scale the pressure (i.e., the fluid density)
  // bcleft:           beam boundary condition on the left
  // x0:               initial location of the left end of the beam (x)
  // y0:               initial location of the left end of the beam (y)
  // useExactSolution: This flag sets the beam model to use the initial conditions
  //                   from the exact solution (FSI) in the documentation.
  // 
  void setParameters(real momOfIntertia, real E, 
		     real rho,real beamLength,
		     real thickness,real pnorm,
		     int nElem,BoundaryCondition bcleft,
		     BoundaryCondition bcright, 
		     real x0, real y0,
		     bool useExactSolution);

  // Return the displacement of the point on the surface (not the neutral axis)
  // of the beam of the point whose undeformed location is (x0,y0).
  // This function is used to update the boundary of the CFD grid.
  // X:       current beam solution vector
  // x0:      undeformed location of the point on the surface of the beam (x)
  // y0:      undeformed location of the point on the surface of the beam (y)
  // x [out]: deformed location of the point on the surface of the beam (x)
  // y [out]: deformed location of the point on the surface of the beam (y)
  //
  void projectDisplacement(const RealArray& X, const real& x0,
			   const real& y0, real& x, real& y);


  // Return the acceleration of the point on the surface (not the neutral axis)
  // of the beam of the point whose undeformed location is (x0,y0).
  // This function is used to enforce the pressure boundary condition for the fluid
  // x0:       undeformed location of the point on the surface of the beam (x)
  // y0:       undeformed location of the point on the surface of the beam (y)
  // ax [out]: acceleration of the point on the surface of the beam (x)
  // ay [out]: acceleration of the point on the surface of the beam (y)
  //
  void projectAcceleration(const real& x0,
			   const real& y0, real& ax, real& ay);

  
  // Accumulate a pressure force to the beam from the fluid element whose 
  // undeformed location is X1 = (x0_1, y0_1), X2 = (x0_2, y0_2).
  // The pressure is p(X1) = p1, p(X2) = p2
  // x0_1: undeformed location of the point on the surface of the beam (x1)  
  // y0_1: undeformed location of the point on the surface of the beam (y1)
  // p1:   pressure at the point (x1,y1)
  // nx_1: normal at x1 (x) [unused]
  // ny_1: normal at x1 (y) [unused]
  // x0_2: undeformed location of the point on the surface of the beam (x2)  
  // y0_2: undeformed location of the point on the surface of the beam (y2)  
  // p2:   pressure at the point (x2,y2)
  // nx_2: normal at x2 (x) [unused]
  // ny_2: normal at x2 (y) [unused]
  //
  void addForce(const real& x0_1, const real& y0_1,
		real p1,const real& nx_1,const real& ny_1,
		const real& x0_2, const real& y0_2,
		real p2,const real& nx_2,const real& ny_2);

  // Set the current force on the beam to zero.  Typical usage is to call
  // resetForce() to zero the force, then call addForce() for every fluid
  // element to accumulate the pressure load, then call predictor()/corrector()
  // to recompute the acceleration.
  //
  void resetForce();

  // Predict the structural state at t^{n+1}, using the Newmark beta predictor.
  // The predictor is only first order accurate.
  // dt:  current time step
  // x1:  solution state (position) at t^{n-1} [unused]
  // v1:  solution state (velocity) at t^{n-1} [unused]
  // x2:  solution state (position) at t^n
  // v2:  solution state (velocity) at t^n
  // x3:  solution state (position) at t^{n+1} [out]
  // v3:  solution state (velocity) at t^{n+1} [out]
  //
  void predictor(real dt, const RealArray& x1, const RealArray& v1, 
		 const RealArray& x2, const RealArray& v2,
		 RealArray& x3, RealArray& v3);

  // Apply the corrector at t^{n+1}
  // dt:  current time step
  // x3:  solution state (position) at t^{n+1} [out]
  // v3:  solution state (velocity) at t^{n+1} [out]
  //
  void corrector(real dt,
		 RealArray& x3, RealArray& v3);

  // Return the current position of the structure.
  //
  const RealArray& position() const;

  // Return the current velocity of the structure.
  //
  const RealArray& velocity() const;

  // Returns true if the fixed point iteration to alleviate
  // the added mass effect has converged.
  //
  bool hasCorrectionConverged() const;

  // Return the exact velocity of the FLUID for the analytical solution
  // derived in the documentation
  // (x,y):      point in the fluid grid where the exact velocity is desired
  // t:          Time at which to compute the exact solution
  // k:          Wave number for the exact solution being computed
  // H:          Height of the fluid domain
  // omega_real: real part of the angular frequency (see documentation)
  // omega_imag: imaginary part of the angular frequency (see documentation)
  // omega0:     Natural (free) frequency of the beam
  // nu:         fluid kinematic viscosity
  // what:       magnitude of the beam deformation
  // u:          fluid velocity (x) [out]
  // v:          fluid velocity (y) [out]
  //
  static void exactSolutionVelocity(real x, real y,real t,
				    real k, real H, 
				    real omega_real, real omega_imag,
				    real omega0, real nu,
				    real what,
				    real& u, real& v);

  // Return the exact pressure of the FLUID for the analytical solution
  // derived in the documentation
  // (x,y):      point in the fluid grid where the exact velocity is desired
  // t:          Time at which to compute the exact solution
  // k:          Wave number for the exact solution being computed
  // H:          Height of the fluid domain
  // omega_real: real part of the angular frequency (see documentation)
  // omega_imag: imaginary part of the angular frequency (see documentation)
  // omega0:     Natural (free) frequency of the beam
  // nu:         fluid kinematic viscosity
  // what:       magnitude of the beam deformation
  // p:          fluid pressure (x) [out]
  //
  static void exactSolutionPressure(real x, real y,real t,
				    real k, real H, 
				    real omega_real, real omega_imag,
				    real omega0, real nu,
				    real what,
				    real& p);

  // Get the exact position, velocity, and acceleration of the beam
  // from the analytical solution derived in the documentation
  // t: Time at which to compute the exact solution
  // x: Position of the beam
  // v: Velocity of the beam
  // a: Acceleration of the beam
  //
  void setExactSolution(double t,RealArray& x, RealArray& v, RealArray& a);

  // Get the exact pressure to be applied to the beam
  // from the analytical solution derived in the documentation
  // t: Time at which to compute the exact pressure
  // x: Location on the beam to get the pressure
  //
  double getExactPressure(double t, double x);

  // Set the relaxation parameter used in the fixed point iteration
  // used to alleviate the added mass effect
  // omega: relaxation factor (default is 1.0)
  //
  void setAddedMassRelaxation(double omega);
  
  // Set the (relative) convergence tolerance for the fixed point iteration
  // tol: convergence tolerance (default is 1.0e-3)
  //
  void setSubIterationConvergenceTolerance(double tol);

  // Allow the beam to undergo free motion.  
  // x0:    initial center of mass of the beam (x)
  // y0:    initial center of mass of the beam (y)
  // angle: initial angle of the beam
  //
  void setupFreeMotion(real x0,real y0, real angle0);

  // Add a constant body force to the beam.  This body
  // "force" has units of acceleration, e.g., g
  // i.e., it is not scaled by the density
  // bf: body force
  //
  void addBodyForce(const real bf[2]);

  // Set the initial angle of the beam, from the x axis
  // dec: angle
  //
  void setDeclination(real dec);

 private:

  // Compute the internal force in the beam, i.e., -K*u
  // u: position of the beam
  // f: internal force [out]
  //
  void computeInternalForce(const RealArray& u,RealArray& f);

  // Compute the integral of N(eta)*p, that is, the rhs
  // of the FEM model, for a particular element
  // p1:   pressure at the first point within the element
  // p2:   pressure at the second point within the element
  // eta1: location (natural coordinate)
  // eta2: location (natural coordinate)
  // fe:   element external force vector [out]
  //
  void computeProjectedForce(real p1, real p2, 
			     real eta1, real eta2,
			     RealArray& fe);

  // Compute the acceleration of the beam.
  // u:               current beam position
  // v:               current beam velocity
  // f:               external force on the beam
  // A:               matrix by which the acceleration is multiplied
  //                  (e.g., in the newmark beta correction step it is 
  //                   M+beta*dt^2*K)
  // a:               beam acceleration [out]
  // linAcceleration: acceleration of the CoM of the beam (for free motion) [out]
  // omegadd:         angular acceleration of the beam (for free motion) [out]
  // dt:              time step
  // locbeta:         [unused]
  // locgamma:        [unused]
  //
  void computeAcceleration(const RealArray& u, const RealArray& v, 
			   const RealArray& f,
			   const RealArray& A,
			   RealArray& a,
			   real linAcceleration[2],
			   real& omegadd,real dt,
			   real locbeta = 0.0,
			   real locgamma = 0.0);

  // Return the element, thickness, and natural coordinate for
  // a point (x0,y0) on the undeformed surface of the beam
  // x0:        undeformed location of the point on the surface of the beam (x)
  // y0:        undeformed location of the point on the surface of the beam (y)
  // elemNum:   element corresponding to this point [out]
  // eta:       natural coordinate corresponding to this point [out]
  // thickness: thickness of the beam at this point [out]
  //
  void projectPoint(const real& x0,const real& y0,
		    int& elemNum, real& eta, real& thickness);

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

  // Compute the third derivative, w'''(x), of the beam displacement w(x) at a given
  // element # and coordinate
  // X:       Beam solution (position)
  // elemNum: element number on which the solution is desired
  // eta:     element natural coordinate where the solution is desired
  // deriv3:  Third derivative, w'''(x) at this point
  //
  void interpolateThirdDerivative(const RealArray& X,
				  int& elemNum, real& eta,
				  real& deriv3);

  // For internal use with free motion.  Recomputes the normal and tangent vectors
  // for the beam (based on the current angle)
  //
  void recomputeNormalAndTangent();

  // Multiply a vector w by the mass matrix
  // w:  vector
  // Mw: M*w [out]
  //
  void multiplyByMassMatrix(const RealArray& w, RealArray& Mw);

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
  real beamX0, beamY0, beamInitialAngle;

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
  int time_step_num;

  // Arrays used to store the forces on the beam
  //
  RealArray myForce,tmp,flocal;

  // Acceleration of the beam (current), and the last computed
  // acceleration of the beam
  //
  RealArray myAcceleration, aold;

  // Current time
  //
  real t;

  // Current Position and velocity of the beam
  //
  RealArray myPosition, myVelocity;

  // Position and velocity of the beam at the previous time step
  //
  RealArray myPosition_nm1, myVelocity_nm1;
  
  // Predicted position/velocity of the beam
  //
  RealArray dtilde,vtilde;

  // Parameters for the newmark beta scheme 
  //
  real newmarkBeta, newmarkGamma;

  // File to which the tip displacement is written
  //
  std::ofstream output;

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
  bool usesExactSolution;

  // The relaxation parameter used in the fixed point iteration
  // used to alleviate the added mass effect
  //
  double added_mass_relaxation;

  // The (relative) convergence tolerance for the fixed point iteration
  // tol: convergence tolerance (default is 1.0e-3)
  //
  double convergenceTolerance;

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
};

#endif
