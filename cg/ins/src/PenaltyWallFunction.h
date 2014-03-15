#ifndef PENALTY_WALL_FUNCTION_H
#define PENALTY_WALL_FUNCTION_H

#include "PenaltySlipWallBC.h"

Parameters::BCModifier * createPenaltyWallFunctionBC(const aString &name);

// Penalty method based implementation of wall functions
// 120105: kkc initial implementation

// These boundary conditions behave just like slip walls except that the normal derivatives of the
// tangential components are given by the shear stress computed using one of a variety of wall models.

class PenaltyWallFunctionBC : public PenaltySlipWallBC {

 public:
  enum WallFunctions {
    slipWall, // mostly for testing
    logLaw,
    simpleLogLaw,
    wernerWengle,
    fixedUTau,
    numberOfWallFunctions
  };

 public:
  PenaltyWallFunctionBC(const aString &nm);

  virtual ~PenaltyWallFunctionBC();

  virtual bool inputFromGI(GenericGraphicsInterface &gi);
  virtual bool applyBC(Parameters &parameters, 
		     const real & t, const real &dt,
		     realMappedGridFunction &u,
		     const int & grid,
		     int side0 /* = -1 */,
		     int axis0 /* = -1 */,
		     realMappedGridFunction *gridVelocity = 0);

  virtual void getShearStresses(const int &nd,
				Parameters &parameters,
				const real &distance,
				const ArraySimpleFixed<real,3,1,1,1> &uavg,
				const ArraySimpleFixed<real,3,1,1,1> tng[],
				ArraySimpleFixed<real,2,1,1,1> &tau);

  real fixedWallDistance, linearLayerTopYPlus;
  WallFunctions wallFunctionType;
  bool includeArtificialDissipationInShearStress;
  bool useFullVelocity;
};

#endif
