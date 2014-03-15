#ifndef APPROXIMATE_FACTORIZATION_H
#define APPROXIMATE_FACTORIZATION_H

#include <vector>
#include <string>
#include <list>
#include "DBase.hh"

namespace CG_ApproximateFactorization {

  class Factor {
  public:
    Factor(const int &dir=0, const std::string nm = "Factor") : direction(dir), name(nm){}
    virtual ~Factor() {}

    virtual void solveRightHandSide(const real &dt, const GridFunction &u, GridFunction &u_star)=0;
    virtual void solveLeftHandSide(const real &dt, const GridFunction &u, GridFunction &u_star)=0;
    virtual void addExplicitContribution(const real &dt, const GridFunction &u, realCompositeGridFunction &f)=0;
    inline int getDirection() const { return direction; }
    inline std::string getName() const { return name;}

  protected:
    std::string name;
  private:
    int direction;
  };

  typedef KK::sptr<Factor> Factor_P;
  typedef std::vector<Factor_P> FactorList;

  extern const int parallelBC;
}

#endif
