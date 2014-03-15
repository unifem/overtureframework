#ifndef INSFACTORS_H
#define INSFACTORS_H

#include "InsParameters.h"
#include "ApproximateFactorization.h"

#define USE_COMBINED_FACTORS

namespace CGINS_ApproximateFactorization {

  enum InsFactorModes {
    solveRHS=0,
    solveLHS,
    addExplicit
  };
#if 0
  enum SpaceApproximations {
    finite_difference=0,
    compact
  };
#endif
  enum FactorTypes {
    R_Factor=0,
    RR_Factor,
    Diagonal_Factor,
    Merged_Factor,
    numberOfFactorTypes
  };

  class INS_Factor : public CG_ApproximateFactorization::Factor {
  public:
    INS_Factor(const int dir, const FactorTypes ft, const InsParameters &parameters_);
    virtual ~INS_Factor();

    virtual void solveRightHandSide(const real &dt, const GridFunction &u, GridFunction &u_star);
    virtual void solveLeftHandSide(const real &dt, const GridFunction &u, GridFunction &u_star);
    virtual void addExplicitContribution(const real &dt, const GridFunction &u, realCompositeGridFunction &f);

  private:
    // disallow construction w/o an INS parameters object
    INS_Factor() : CG_ApproximateFactorization::Factor(0), type(R_Factor), parameters(0)  {}
    const FactorTypes type;
    const InsParameters *parameters;
  };

};

#endif
