//                                   -*- c++ -*-
#ifndef DEFORMING_BODY_MOTION_H
#define DEFORMING_BODY_MOTION_H
 
//
// Master class for keeping track of deforming bodies in flow:
//   The particular physics of the object is determined by
//   a separate class that handles the evolution of the body
//   under the surface stresses arising from the fluid:
//

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

// Forward defs for the physicsObjects
class ElasticFilament;
class BeamModel;
class NonlinearBeamModel;


//................................
class DeformingBodyMotion
{
 
public:

friend class Parameters;
friend class AdParameters;
friend class AsfParameters;
friend class CnsParameters;
friend class InsParameters;
friend class SmParameters;

enum DeformingBodyType
{
  elasticFilament,
  //elasticRod,
  // elasticShell,
  elasticBody,
  userDefinedDeformingBody,
  unknownBody
};

typedef ArraySimpleFixed<int,2,2,1,1> BcArray;
enum DeformingBoundaryBoundaryConditionEnum
{
  periodicBoundaryCondition=0,
  dirichletBoundaryCondition,
  neumannBoundaryCondition,
  slideBoundaryCondition,
  numberOfBoundaryConditions // counts number of extries
};

  

enum InitialStateOptionEnum
{
  initialPosition,
  initialVelocity,
  initialAcceleration
};




DeformingBodyMotion(Parameters & params, 
		    int numberOfTimeLevels               = 3,
		    GenericGraphicsInterface *pGIDebug00 =NULL,
		    int debug00                          =0);

~DeformingBodyMotion();

int buildElasticShellOptionsDialog(DialogData & dialog );
int buildElasticBeamOptionsDialog(DialogData & dialog );

// apply correction at time t using new values of the forces at time t.
int correct( real t1, real t2, 
	     GridFunction & cgf1,GridFunction & cgf2 );

// define faces and grids that form the deforming body 
int defineBody( int numberOfFaces, IntegerArray & boundaryFaces );

const IntegerArray & getBoundaryFaces() const;

// Construct a grid from the past time, needed to start some PC schemes.
int getPastTimeGrid(  real pastTime , CompositeGrid & cg );

int getAccelerationBC( const real time0, const int grid, MappedGrid & mg, 
		       const Index &I1, const Index &I2, const Index &I3, 
		       realSerialArray & bcAcceleration);

// return the order of accuracy used to compute the acceleration 
int getAccelerationOrderOfAccuracy() const;

int getBodyVolumeAndSurfaceArea( CompositeGrid & cg, real & volume, real & area );

// Return the beamModel (if it exists)
BeamModel& getBeamModel();

int getElasticShellOption(const aString & answer, DialogData & dialog );
int getElasticBeamOption(const aString & answer, DialogData & dialog );

// return the initial state (position, velocity, acceleration)
int getInitialState( InitialStateOptionEnum stateOption, 
		     const real time, 
                     const int grid, MappedGrid & mg, const Index &I1, const Index &I2, const Index &I3, 
		     realSerialArray & state );

int getNumberOfGrids(); 


int getVelocity( const real time0, 
		 const int grid, 
		 CompositeGrid & cg,
		 realArray & gridVelocity);


int getVelocityBC( const real time0, const int grid, MappedGrid & mg, const Index &I1, const Index &I2, const Index &I3, 
		   realSerialArray & bcVelocity);

// return the order of accuracy used to compute the velocity 
int getVelocityOrderOfAccuracy() const;



//..Grid position, velocity & boundary acceleration

int initialize(CompositeGrid & cg, real t=0. );

int initializeGrid(CompositeGrid & cg, real t=0. ); 

int initializePast( real time00, real dt00, CompositeGrid & cg);

// return true if the deforming body is a beam model
bool isBeamModel() const;

// integrate the BODY to a new time
int integrate( real t1, real t2, real t3, 
	       GridFunction & cgf1,GridFunction & cgf2,GridFunction & cgf3,
	       realCompositeGridFunction & stress );

// --- plot things related to moving grids (e.g. the center lines of beams or shells)
int plot(GenericGraphicsInterface & gi, GridFunction & cgf, GraphicsParameters & psp );

void printFilamentHyperbolicDimensions(CompositeGrid & cg00, int gridToMove00);

int regenerateComponentGrids( const real newT, CompositeGrid & cg);

void registerDeformingComponentGrid( const int grid, CompositeGrid & cg);

// set the order of accuracy used to compute the acceleration
int setAccelerationOrderOfAccuracy( int order );

// set the order of accuracy used to compute the velocity
int setVelocityOrderOfAccuracy( int order );

int setType( const DeformingBodyType bodyType );

// interactive update
int update(CompositeGrid & cg, GenericGraphicsInterface & gi );
int update(  GenericGraphicsInterface & gi );


// user defined deforming surface: 
int userDefinedDeformingSurface(real t1, real t2, real t3, 
				GridFunction & cgf1,
				GridFunction & cgf2,
				GridFunction & cgf3,
				int option );

bool hasCorrectionConverged() const;

int get( const GenericDataBase & dir, const aString & name);

int put( GenericDataBase & dir, const aString & name) const;

protected: 

int advanceElasticShell(real t1, real t2, real t3, 
			GridFunction & cgf1,
			GridFunction & cgf2,
			GridFunction & cgf3,
			realCompositeGridFunction & stress,
			int option );

int advanceElasticBeam(real t1, real t2, real t3, 
		       GridFunction & cgf1,
		       GridFunction & cgf2,
		       GridFunction & cgf3,
		       realCompositeGridFunction & stress,
		       int option );

int advanceNonlinearBeam(real t1, real t2, real t3, 
			 GridFunction & cgf3,
			 realCompositeGridFunction & stress,
			 int option );

  int getFace( int grid ) const;

  int getPastLevelGrid( const int level, 
		  const int grid, 
		  CompositeGrid & cg,
		  realArray & gridVelocity);

  void simpleGetVelocity( const real vTime, 
			  const int grid00, 
			  CompositeGrid & cg,
			  realArray & gridVelocity);



  ElasticFilament *pElasticFilament;  // the Mapping for an elastic filament 
  DeformingGrid *pDeformingGrid;      // The "grid" associated with the elastic filament

  BeamModel* pBeamModel;

  NonlinearBeamModel* pNonlinearBeamModel;

  int debug;
  GenericGraphicsInterface *pGIDebug;
  MappingInformation       *pMapInfoDebug;

  Parameters & parameters;  // parameters from the DomainSolver

  mutable DataBase deformingBodyDataBase;  // save DeformingBodyMotion parameters in here 


};

#endif

