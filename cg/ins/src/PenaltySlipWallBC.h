#ifndef PENALTY_SLIP_WALL_BC_H
#define PENALTY_SLIP_WALL_BC_H

#include "Parameters.h"
#include "DBase.hh"

Parameters::BCModifier * createPenaltySlipWallBC(const aString &name);

class PenaltySlipWallBC : public Parameters::BCModifier {
  
 public:
  PenaltySlipWallBC(const aString &nm);

  virtual ~PenaltySlipWallBC();

  virtual bool inputFromGI(GenericGraphicsInterface &gi);
  virtual bool applyBC(Parameters &parameters, 
		     const real & t, const real &dt,
		     realMappedGridFunction &u,
		     const int & grid,
		     int side0 /* = -1 */,
		     int axis0 /* = -1 */,
		     realMappedGridFunction *gridVelocity = 0);

  virtual bool setBCCoefficients(Parameters &parameters, 
				 const real & t, const real &dt,
				 realMappedGridFunction &u,
				 realMappedGridFunction &coeff,
				 const int & grid,
				 int side0 /* = -1 */,
				 int axis0 /* = -1 */,
				 realMappedGridFunction *gridVelocity = 0);
  

  virtual bool addPenaltyForcing(Parameters &parameters, 
				 const real & t, const real &dt,
				 const realMappedGridFunction &u,
				 realMappedGridFunction &dudt,
				 const int & grid,
				 int side0 /* = -1 */,
				 int axis0 /* = -1 */,
				 const realMappedGridFunction *gridVelocity = 0);

  virtual const bool isPenaltyBC() const;

  // private:
  real normalFlux;
  bool zeroTangentialVelocity;
  DBase::DataBase db;

};
#endif
