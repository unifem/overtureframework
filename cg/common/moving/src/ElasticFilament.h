//                               -*- c++ -*-
#ifndef ELASTIC_FILAMENT_H
#define ELASTIC_FILAMENT_H

// Subclass of DeformingBodyMotion: 
//  * For keeping track of a 1d filament in 2d flow.
//    ---> for the soapfilm-filament flow of Zhang et al.
//
#include "Overture.h"
#include "FilamentMapping.h"
#include "HyperbolicMapping.h"

// Forward Defs:
class GenericGraphicsInterface;
class SplineMapping;
//class DeformingGridGenerationInformation;

class TravelingWaveParameters;   // for keeping debug filament data

class ElasticFilament
{
public:
  
  ElasticFilament(int  nFilamentPoints=17,  // was=81
			int  nEndPoints=3);
  ~ElasticFilament();

  //XXXXXX CHANGE -- set appropriate properties here: bending mod, dens etc
  enum FilamentPropertyFlag { 
    SET_BENDING_PARAMETER, SET_DENSITY, 
    SET_N_FILAMENT_POINTS,SET_N_END_POINTS, 
    SET_END_RADIUS, SET_THICKNESS, SET_LENGTH,
    SET_A_PARAMETER, SET_B_PARAMETER, SET_OMEGA,
    SET_K_NUMBER
  };

  enum FilamentDynamicsType {
    PRESCRIBED_DYNAMICS,
    LINEAR_BEAM_DYNAMICS,
    NONLINEAR_BEAM_DYNAMICS,
    EXTENSIBLE_KIRCHOFF_DYNAMICS,
    INEXTENSIBLE_KIRCHOFF_DYNAMICS
  };

  FilamentDynamicsType filamentDynamicsType;   // default=PRESCRIBED_DYNAMICS
  
  void setProperties( enum FilamentPropertyFlag flag, real value);
  void setProperties( enum FilamentPropertyFlag flag, int value);
  void getProperties( enum FilamentPropertyFlag flag, real &value);
  void getProperties( enum FilamentPropertyFlag flag, int  &value);

  void initialize();
  void initializeSurfaceData(double time);

  //int getGridGenerationInformation( DeformingGridGenerationInformation & dggi);
  HyperbolicMapping *getHyperbolicMappingPointer();

  void regenerateBodyFittedMapping();
  void regenerateBodyFittedMapping( HyperbolicMapping *pHyper00);

  // integrate to a new time
  //int integrate( real tf, const realArray & surfaceStress, real t);
  int integrate( real tf, const RealCompositeGridFunction & surfStress, real t);
private: //--AUX to integrate
  void computePrescribedSurfaceData();
  void computeSurfaceData();
public:

  // correct solution at time t using new values of the forces at time t.
  //int correct( real t, const realArray & surfaceStress);

  //.. Evaluate current surface at time=time0--> extrap/interp if necessary
  int evaluateSurfaceAtTime( real time0 ); 
  void copyBodyFittedMapping( HyperbolicMapping &copyMap,
			      aString *pNewMappingName =NULL);

  void referenceMap( int gridToMove, CompositeGrid &cg);        // DEBUG -- updates the grid?? 10/15/00
  void replaceHyperbolicMapping( HyperbolicMapping *pNewHyper ); //OBSOLETE
  HyperbolicMapping getHyperbolicMapping(); 
  Mapping *getSurface();

  int update( GenericGraphicsInterface & gi );


  //.....for boundary conditions in the flow solver
  //  int getVelocityBC(     const real time0, const int grid,  // not used
  //                       CompositeGrid & cg, realArray & bcAcceleration);
  //int getAccelerationBC( const real time0, const int grid,  // not used
  //                       CompositeGrid & cg, realArray & bcAcceleration);
  //
  int getVelocityBC(     const real time0, 
  			 const Index &I1, const Index &I2, const Index &I3, 
			 realSerialArray & bcVelocity);

  int getAccelerationBC( const real time0, 
			 const Index &I1, const Index &I2, const Index &I3, 
			 realSerialArray & bcAcceleration);

  //private:  ------- AUX ROUTINES

  //Time stepping parameters & variables
  int  numberOfSteps;         // for pred/correct
  int  maximumNumberToSave;   // ...of internal stages in a tstepper
  int  numberSaved;           // ...so far
  RealArray time;             // saved times
  real tcomp;                 // Current time
  int  current;               // current position in arrays

  //MATERIAL PROPERTIES
  real mass;               // total mass
  real density;            // density/unit lenght
  real bendingMoment;      // elastic bending moment
  real el;                 // filament length

  //POSITION & STRESSES
  realSerialArray xAll,vAll,accelAll;   // Filament position & velocity
  realSerialArray rhsAll;               // Rhs of eq. for the filam.
  realSerialArray x0, v0;               // initial data
  realSerialArray surfaceStress;        // surface stress from fluid??

  //..AUX
  void initializeFromFilamentMapping( FilamentMapping *pFilamCopy );

  FilamentMapping *pFilamentMapping;
  bool             isFilamentMappingMine;

  //.......OBSOLETE

  //DEBUG DATA
  int  debug;                 // DEBUG level
  RealArray   xDebug;

  //REPRESENTATION as splines & normal/tang vecs
  int            nFilamentPoints;
  int            nTotalThickFilamentPoints;

  realSerialArray xFilament;                            // filament (x,y) values
  realSerialArray x_t, x_tt, xr, xr_t, xr_tt;
  //RealArray coreVelocity, coreAcceleration;        

  // on thick filament
  realSerialArray xThickFilament;              // All of thickFilam->to Spline
  realSerialArray surfaceVelocity;     real surfaceVelocityTime;
  realSerialArray surfaceAcceleration; real surfaceAccelerationTime;
  realSerialArray stressThickFilament;         // fluid stress on thick filam.

  //int            nSplinePoints;                   //  >= nFilamPts
  //SplineMapping *pFilament;                       // The filament
  //realArray      normalVector, tangentVector;     // Normal & Tangent

  //..Thick Filament
  //real thickness;                          // thickness
  //real endRadius;                          // radius of the end
  //int  nThickSplinePoints;
  //SplineMapping *pThickFilament;           // bdry with fluid

  //int       nEndPoints;
  //int       nTotalThickFilamentPoints;
  //realArray xTop,xBottom;                // Pieces of the thick filam.
  //realArray xLeadingEnd, xTrailingEnd;   // - " -


};
#endif


