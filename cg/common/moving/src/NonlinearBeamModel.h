//                                   -*- c++ -*-
#ifndef NONLINEAR_BEAM_MODEL_H
#define NONLINEAR_BEAM_MODEL_H
 
#include "Overture.h"
#include "HyperbolicMapping.h"
#include "GenericGraphicsInterface.h"
#include "GridEvolution.h"
#include "DBase.hh"
#include <fstream>
using namespace DBase;

// Forward defs:
class GenericGraphicsInterface;
class MappingInformation;
class DeformingGrid;
class GridFunction; 
class Parameters;

//................................
class NonlinearBeamModel {

 public:

  enum BoundaryCondition { UnknownBC=-1, Pinned = 1 , Cantilevered = 2, Free = 4 , Periodic = 8 };

  enum ExactSolutionEnum
  {
    fluidStructureTravelingWave=0,
    standingWave
  };

  struct BeamNode {

    // undeformed location
    real X[3];

    // displacement
    real u[3];

    // 
    real undeformedRotation;
    
    real rotation;

    real p0[2];
    real p[3];

    // beam thickness at this node
    real thickness;

    real v[3];

    real a[3];

    real utilde[3];
    real vtilde[3];
    
    real angularVelocity;

    real angularAcceleration;

    real angletilde;
    real angularVelocitytilde;

    real oldAccelerations[3];
  };

  struct SlaveState {

    
    real uplus[3]; real uminus[3];
    real xplus[3]; real xminus[3];
    real Xplus[3]; real Xminus[3];
    real vplus[3]; real vminus[3];
    real aplus[3]; real aminus[3];
  };

  struct ProjectedPoint {

    int elem;
    real dist;
    real xi;
    real eta;

    real projDelta[2];
  };

  NonlinearBeamModel();

  ~NonlinearBeamModel();



  void addBodyForce(const real[2]);

  void addForce(int i1, real p1, int i2, real p2);

  void corrector(real dt);

  int get( const GenericDataBase & dir, const aString & name);

  // Return the (x,y) coordinates of the centerline
  void getCenterLine( RealArray & xc ) const;

  // return the estimated *explicit* time step dt 
  real getExplicitTimeStep() const;

  int getNumberOfNodes() const { return numNodes; } // 

  // return an estimate of the time-step dt
  real getTimeStep() const;

  bool hasCorrectionConverged() const;

  void initializeProjectedPoints(int sz);

  void predictor(real dt);

  void projectAcceleration(int id, real& ax, real& ay);

  void projectDisplacement(int id, real& x, real& y);

  void projectInitialPoint(int id, real x, real y);
  
  int put( GenericDataBase & dir, const aString & name) const;


  int plot(GenericGraphicsInterface & gi, GraphicsParameters & psp );

  void readBeamFile(const char* filename);

  void resetForce();

  void setAddedMassRelaxation(double);

  void setExactSolution(double t,RealArray& x, RealArray& v, RealArray& a);

  // Set a real beam parameter (in the dbase). 
  int setParameter( const aString & name, real & value );

  void setSubIterationConvergenceTolerance(double tol);

  // Set parameters interactively: 
  int update(CompositeGrid & cg, GenericGraphicsInterface & gi );

  // Output model parameters 
  void writeParameterSummary( FILE *file= stdout );

  static int debug;

  /// Here is a database to hold parameters (new way)
  mutable DataBase dbase; 

 private:

  void computeShapeFunctions(real xi, real eta, real N[4]);
  void computeShapeFunctionGradients(real xi, real eta, real N[4][2]);

  void computeSlaveStates();


  void computeLaminarBasis(int elem,real xi, real eta,
			   real R[4], real ex[2],
			   real ey[2],
			   real Rdef[4]);

  void computeLaminarComponents(int elem,real xi, real eta, real R[4],
				real F[4], real E[4], real& J,
				real Nx[4][2], real& Jeta);

  void computeStressSVK(int elem,real xi, real eta, real R[4],
			real F[4], real E[4], real J,
			real sigma[4]);

  void computeInternalForce(int elem,RealArray& F,
			    RealArray& Kelem);

  void computeInternalForce(RealArray& F,RealArray& KT);

  void computeElementalMassMatrix(int elem, RealArray& M);

  void computeMassMatrix(int elem,RealArray& MT);

  void reevaluateMassMatrix();

  void computeBodyForce(RealArray& Fout);
  void computeBodyForce(int, RealArray& Fout);

  void computeMaterialStiffness(real cg[2][2],
				real Nxi, real Nyi,
				real Nxj, real Nyj);

  void computeGeometricStiffness(real kgeo[2][2],
				 real sigma[4],
				 real Nxi, real Nyi,
				 real Nxj, real Nyj) ;

  void initialize();

  double getExactPressure(double t, double xl);
  
  void computeExtraInertiaTerm(RealArray&) ;

  BeamNode* beamNodes;

  SlaveState* slaveStates;

  ProjectedPoint* projectedPoints;

  int numProjectedPoints;

  real density, nu, Em;

  int numNodes,numElem;

  // Mass matrices
  RealArray M;

  // Stiffness Matrices
  RealArray K;

  real newmarkBeta,newmarkGamma;

  real omegaStructure;

  BoundaryCondition bcLeft, bcRight;

  real bodyForce[2];

  RealArray Fext,Ffluid;

  int isSteady;

  real t;

  std::ofstream tipfile;

  real pressureNorm;

  int useExactSolution;
  ExactSolutionEnum exactSolution;

  int time_step_num;

  bool correctionHasConverged;

  double added_mass_relaxation;

  double convergenceTolerance;

  double initialResidual;

  int numCorrectorIterations;

  RealArray* elementMassMatrices;

  double rayleighAlpha, rayleighBeta;

  double beamLength;
  double beamThickness;

  aString name;  // name of this object


};

#endif
