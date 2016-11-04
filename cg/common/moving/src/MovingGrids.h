//                              -*- c++ -*-
#ifndef MOVING_GRIDS_H
#define MOVING_GRIDS_H

#include "Overture.h"

//..Forward definitions
class GridFunction;
class GenericGraphicsInterface;
class RigidBodyMotion;
class Parameters;
class Integrate;
class Ogshow;

class DeformingBodyMotion;
class MatrixMotion;
class BodyForce;

// ========================================================================
//    This class coordinates the movement of grids for CG
// ========================================================================

class MovingGrids
{
public:

// This enum defines how the grid for a body moves
enum MovingGridOption
{
  notMoving,
  rotate,
  shift,
  oscillate,
  scale,
  matrixMotion,  // pre-defined matrix (rigid or scaling) motions 
  rigidBody,         // rigid motions defined by fluid forces and torques 
  deformingBody,
  userDefinedMovingGrid,
  numberOfMovingGridOptions
};

  

MovingGrids(Parameters & parameters);
MovingGrids(const MovingGrids & mg);
virtual ~MovingGrids();


// Assign initial conditions and past time state
int assignInitialConditions( GridFunction & cgf );

// Compute rigid body properties such as the mass and moments of inertia
int computeRigidBodyProperties( const int bodyNumber, CompositeGrid & cg );

// correction step: 
virtual int correctGrids(const real t1,
			 const real t2, 
			 GridFunction & cgf1,
			 GridFunction & cgf2 );

static int debug0;
int debug() const {return debug0;}

int detectCollisions(GridFunction & cgf1);
  
// for plotting purposes return moving bodies as BodyForce objects (these are saved to the show file)
int getBodies( std::vector<BodyForce*> & movingBodies );

virtual int getBoundaryAcceleration( MappedGrid & c, realSerialArray & gtt, int grid, real t0, int option );

bool getCorrectionHasConverged();

// return the mass of the fluid that would be contained in body b.
real getFluidMassOfBody( int b ) const;

// Return the integrated fluid force and torque on a rigid body
int getForceOnRigidBodies( RealArray & force, RealArray & torque, GridFunction & gf0,
			   bool includeGravity = true, 
			   bool includeViscosity = true );

virtual int getGridVelocity( GridFunction & gf0, const real & tGV );

Integrate* getIntegrate() const;

const RealArray & getMoveParameters() const;

// return maximum relative correction for sub-iterations
real getMaximumRelativeCorrection() const;

int getNumberOfDeformingBodies() const;

DeformingBodyMotion & getDeformingBody(const int bodyNumber);

int getNumberOfMatrixMotionBodies() const;

MatrixMotion & getMatrixMotionBody(const int bodyNumber);

int getNumberOfRigidBodies() const;

// Construct a grid in the past (used for starting a multi-step scheme for e.g.)
int getPastTimeGrid( GridFunction & cgf );

RigidBodyMotion & getRigidBody(const int bodyNumber);

int getRigidBodyAddedDampingTensors( const int bodyNumber, RealArray & addedDampingTensors, 
                                     GridFunction & cgf, const real dt );

real getTimeStepForMovingBodies() const;

real getTimeStepForRigidBodies() const; 

int getUserDefinedBoundaryAcceleration( MappedGrid & mg, realSerialArray & gtt, int grid, real t0, int option,
					  const int side, const int axis );

int getUserDefinedGridVelocity( GridFunction & gf0, const real & t0, const int grid );

// get from a data base file
int get( const GenericDataBase & dir, const aString & name);

virtual int gridAccelerationBC(const int grid, const int side, const int axis,
			       const real t0,
			       MappedGrid & c,
			       realMappedGridFunction & u ,
			       realMappedGridFunction & f ,
			       realMappedGridFunction & gridVelocity ,
			       realSerialArray & normal,
			       const Index & I1,
			       const Index & I2,
			       const Index & I3,
			       const Index & I1g,
			       const Index & I2g,
			       const Index & I3g    );

bool gridIsMoving(int grid) const;

 
virtual int moveDeformingBodies(const real & t1, 
				const real & t2, 
				const real & t3,
				const real & dt0,
				GridFunction & cgf1,  
				GridFunction & cgf2,
				GridFunction & cgf3 );

virtual int moveGrids(const real & t1, 
		      const real & t2, 
		      const real & t3,
		      const real & dt0,
		      GridFunction & cgf1,  
		      GridFunction & cgf2,
		      GridFunction & cgf3 );
  
MovingGridOption movingGridOption(int grid) const;

aString movingGridOptionName(MovingGridOption option) const;
  

bool isMovingGridProblem() const;

// output probe info
int outputProbes( GridFunction & gf0, int stepNumber );

// --- plot things related to moving grids (e.g. the center lines of beams or shells)
int plot(GenericGraphicsInterface & gi, GridFunction & cgf, GraphicsParameters & psp );

// print time step info
void printTimeStepInfo( FILE *file=stdout );

// Project the interface velocity (for added mass schemes)
int projectInterfaceVelocity( GridFunction & cgf );

// put to a data base file
int put( GenericDataBase & dir, const aString & name) const;

virtual int rigidBodyMotion(const real & t1, 
			    const real & t2, 
			    const real & t3,
			    const real & dt0,
			    GridFunction & cgf1,  
			    GridFunction & cgf2,
			    GridFunction & cgf3 );

virtual int saveToShowFile() const;
  
int setIsMovingGridProblem( bool trueOrFalse=TRUE );

// interactive update
virtual int update(CompositeGrid & cg, GenericGraphicsInterface & gi );



int updateToMatchGrid( CompositeGrid & cg );

// general motion
int userDefinedMotion(const real & t1, 
		      const real & t2, 
		      const real & t3,
		      const real & dt0,
		      GridFunction & cgf1,  
		      GridFunction & cgf2,
		      GridFunction & cgf3 );
  
// motions that use a MatrixTransform -- shift, rotate, scale
int userDefinedTransformMotion(const real & t1, 
			       const real & t2, 
			       const real & t3,
			       const real & dt0,
			       GridFunction & cgf1,  
			       GridFunction & cgf2,
			       GridFunction & cgf3,
			       const int grid );
  
int userDefinedGridAccelerationBC(const int & grid,
				  const real & t0,
				  MappedGrid & c,
				  realMappedGridFunction & u ,
				  realMappedGridFunction & f ,
				  realMappedGridFunction & gridVelocity ,
				  realSerialArray & normal,
				  const Index & I1,
				  const Index & I2,
				  const Index & I3,
				  const Index & I1g,
				  const Index & I2g,
				  const Index & I3g );

int updateUserDefinedMotion(CompositeGrid & cg, GenericGraphicsInterface & gi);

// Write information to the `check file' 
int writeCheckFile( real t, FILE *file );

// Write information about the moving grids
void writeParameterSummary( FILE *file= stdout );

protected:

int getRamp(real t, real rampInterval, real & ramp, real & rampSpeed, real & rampAcceleration );

int initialize();
  
int initializeMovingGridTransforms( GridFunction & cgf );

Parameters & parameters;

IntegerArray gridsToMove;

bool isInitialized;

bool movingGridProblem;
IntegerArray moveOption;
IntegerArray movingGrid;         // gridIsMoving(grid) is true if a grid is moving
RealArray moveParameters;          // moveParameters(.,grid) : parameters for the type of movement

// Matrix Motion body info 
int numberOfMatrixMotionBodies;
MatrixMotion **matrixMotionBody;  // array of pointers to one or more matrix motion bodies

//..Rigid body information
int numberOfRigidBodies;
RigidBodyMotion **body;     // array of pointers one or more rigid bodies.
Integrate *integrate;       // used to integrate stresses on bodies.
bool useHybridGridsForSurfaceIntegrals;

int rigidBodyInfoCount;
RealArray rigidBodyInfo;    // save time sequence of some rigid body info.
RealArray rigidBodyInfoTime;
  
int numberOfRigidBodyInfoNames;
aString *rigidBodyInfoName;

bool limitForces;
real maximumAllowableForce, maximumAllowableTorque;

bool correctionHasConverged;
real maximumRelativeCorrection;

bool recomputeGridVelocityOnCorrection;

//..Deforming body information
int numberOfDeformingBodies;
DeformingBodyMotion **deformingBodyList;
  
};

#endif
