#include <cassert>

#include "MultiComponent.h"

using namespace std;
using namespace CG;

namespace {

  CG::Mixture *current_mixture = 0;

}

void
CG::
setMixtureContext(Mixture &mixture) 
{
  current_mixture = &mixture;
}

Mixture *
CG::
getMixtureContext()
{
  return current_mixture;
}

extern "C" void cgmc_get_gas_coefficients_(const int &nspc, const real_t *state, 
					   real_t &mavg, real_t &cp, real_t &cv, real_t &mu, real_t &kth)
{
  const real_t density = state[0];
  const real_t temperature = state[1];
  const real_t *X = state + 2;

  assert(current_mixture);

  mavg = current_mixture->getMAvg(X);
  cp   = current_mixture->getCp(density,temperature,X);
  cv   = current_mixture->getCv(density,temperature,X);
  mu   = current_mixture->getViscosity(density,temperature,X);
  kth  = current_mixture->getKThermal(density,temperature,X);

  return;
}

extern "C" void cgmc_get_mc_diffusivities_(const int &nspc, const real_t *state, real_t *mcd)
{
  const real_t density = state[0];
  const real_t temperature = state[1];
  const real_t *X = state + 2;

  assert(current_mixture);
  assert(mcd);
  current_mixture->getDiffusionCoefficients(temperature,density,X,mcd);

  return;
}

extern "C" void cgmc_get_species_gas_coefficients_(const int &ispc, const real_t &density, const real_t &temperature, 
						   real_t &mw, real_t &cp, real_t &cv, real_t &mu, real_t &kth)
{
  assert(current_mixture);

  const Material &mat = current_mixture->getMaterial(ispc);

  mw = mat.getMWeight();
  cp = mat.getCp(temperature,density);
  cv = mat.getCv(temperature,density);
  mu = mat.getViscosity(temperature,density);
  kth= mat.getKThermal(temperature,density);
  
  return;
}
