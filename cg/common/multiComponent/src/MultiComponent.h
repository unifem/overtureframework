#ifndef __CG_MULTI_COMPONENT_H__
#define __CG_MULTI_COMPONENT_H__

// 090401 kkc : initial version
//
// This file defines classes that support the computation of molecular and thermodynamic coefficients
// of multi-component mixtures of substances.
// As of 090401 it only supports mixtures of ideal gases.
//
// Notes : - should this be morphed into a general EOS package?  kkc has a general eos package sitting around...
//         - What other kinds of mixtures should we support? multi-phase? liquid?
//         - Units are assumed to be meters-kilograms-seconds
//         - but this assumption only really affects Material::getCv, we should probably allow users to set the units by setting the gas constant?

#include <vector>
#include <string>
#include "kk_ptr.hh"

#include "CgTypes.h"
#include "CgConstants.h"
#include "CgInteractionPotential.h"

class GenericGraphicsInterface;

namespace CG {

  class Material;
  class Mixture;

  typedef KK::sptr<Mixture> MixtureP;
  typedef KK::sptr<Material> MaterialP;

  // 
  // A Material describes the molecular and thermodynamic properties of a substance.
  // Right now it only supports Perfect Gas type materials... It should be a base class?
  //
  class Material
  {
  public:
    Material() {}
    Material(std::string nm, real_t m_weight, real_t Cp, real_t k_thermal, real_t viscosity);
    Material( const Material &mat );

    virtual ~Material() {}

    Material &operator=(const Material &mat);

    inline real_t getMWeight() const { return M;}
    inline virtual real_t getCp(const real_t &temp, const real_t &den) const { return c_p;}
    inline virtual real_t getCv(const real_t &temp, const real_t &den) const { return c_p - CG::MKS::gas_constant/M;} // !!! MKS UNITS ASSUMPTION !!!
    inline virtual real_t getKThermal(const real_t &temp, const real_t &den) const { return k_th;}
    inline virtual real_t getViscosity(const real_t &temp, const real_t &den) const { return mu;}
    inline const std::string &getName() const { return name; }

    // the following macro is setup in CgInteractionPotential.h
    CG_DEFINE_INTERACTION_POTENTIAL_MATERIAL_INTERFACE
  private:
    real_t M, c_p, k_th, mu;
    std::string name;

    // the following macro is setup in CgInteractionPotential.h
    CG_DEFINE_INTERACTION_POTENTIAL_MATERIAL_DATA
  };


  // 
  // A Mixture manages a collection of Materials. It is an abstract base class because
  // the way materials combine to form Mixture coefficients like Cp and Cv depends on the
  // mixture model.
  //
  class Mixture
  {
  public:
    typedef std::vector<Material> MaterialList;
    typedef MaterialList::iterator mat_iterator;
    typedef MaterialList::const_iterator const_mat_iterator;

    Mixture();
    virtual ~Mixture();

    // 
    // constituent material query and manipulation methods
    //
    bool addMaterial(const CG::Material &mat);
    inline const Material &getMaterial(const int mat) const { return mat_list[mat]; }

    inline mat_iterator begin() { return mat_list.begin(); }
    inline const_mat_iterator begin() const { return mat_list.begin(); }

    inline mat_iterator end() { return mat_list.end(); }
    inline const_mat_iterator end() const { return mat_list.end(); }

    inline size_t numberOfMaterials() const { return mat_list.size(); }

    //
    // material coefficient computations
    //
    inline virtual real_t getMAvg(const real_t *X)
    { 
      real_t mbar = 0.;
      for ( int c=0; c<numberOfMaterials(); c++ ) mbar += X[c]*mat_list[c].getMWeight();
      return mbar;
    }

    // the following methods are fairly gas-specific, can we generalize them somehow?
    // what else should be in the list?
    virtual real_t getCp(const real_t &temp, const real_t &den, const real_t *X) = 0;
    virtual real_t getCv(const real_t &temp, const real_t &den, const real_t *X) = 0;
    virtual real_t getViscosity(const real_t &temp, const real_t &den, const real_t *X);
    virtual real_t getKThermal(const real_t &temp, const real_t &den, const real_t *X);
    virtual real_t getPressure(const real_t &temp, const real_t &den, const real_t *X) = 0;
    virtual real_t getTemperature(const real_t &press, const real_t &den, const real_t *X) = 0;
    virtual real_t getDensity(const real_t &temp, const real_t &press, const real_t *X) = 0;

    virtual real_t getOmegaIntegral(const Material &mat1, const Material& mat2, const int&i, const int &j, const real_t &temp);
    virtual void getBinaryDiffusionCoefficients(const real_t &temp, const real_t &pressure, real_t *&diffusionCoefficients);
    virtual void getDiffusionCoefficients(const real_t &temp, const real_t &den, const real_t *X, real_t *&diffusionCoefficients);

  private:
    
    MaterialList mat_list;

  };

  //
  // An ideal gas mixture with simple mole- and mass-fraction based averaging rules for Cp, Cv, and R.
  //
  class IdealGasMixture : public Mixture
  {
  public:
    IdealGasMixture() : Mixture() {}
    virtual ~IdealGasMixture() {}

    // base class overloads...
    virtual real_t getCp(const real_t &temp, const real_t &den, const real_t *X);
    virtual real_t getCv(const real_t &temp, const real_t &den, const real_t *X);
    virtual real_t getPressure(const real_t &temp, const real_t &den, const real_t *X) { return MKS::gas_constant*temp*den/Mixture::getMAvg(X); } //!!! MKS UNITS ASSUMPTION !!!
    virtual real_t getTemperature(const real_t &press, const real_t &den, const real_t *X) { return press/(MKS::gas_constant*den/Mixture::getMAvg(X)); } //!!! MKS UNITS ASSUMPTION !!!
    virtual real_t getDensity(const real_t &temp, const real_t &press, const real_t *X) { return press/(MKS::gas_constant*temp/Mixture::getMAvg(X)); } //!!! MKS UNITS ASSUMPTION !!!
    // We use the simple averging rules for the base class to get kthermal and the viscosity.

  }; 

  // 
  // The TwilightZoneMixture provides constant coefficients so that the diffusion 
  //   coefficieints can be used with twilight zone exact solutions.  The user must specify
  //   the temperature, density and mole fraction to evaluate for the coefficient evaluation.
  //
  class TwilightZoneMixture : public IdealGasMixture
  {
  public:
    TwilightZoneMixture() : IdealGasMixture(), tw_temp(-1.0), tw_den(-1) {}
    TwilightZoneMixture(const real_t &temp, const real_t &den, const Mixture &mix, const real_t *X) : IdealGasMixture(), tw_temp(temp), tw_den(den) 
    {
      int i=0;
      for ( Mixture::const_mat_iterator mat=mix.begin(); mat!=mix.end(); mat++,i++ )
	addMaterial(*mat,X[i]);
    }

    virtual ~TwilightZoneMixture() { }

    bool addMaterial(const CG::Material &mat, const real_t &fraction)
    {
      Mixture::addMaterial(mat);
      tw_X.push_back(fraction);
    }

    // overloads of Mixture functions:
    inline virtual real_t getMAvg(const real_t *X)
    { return Mixture::getMAvg(&tw_X[0]); }

    virtual real_t getCp(const real_t &temp, const real_t &den, const real_t *X) 
    { return IdealGasMixture::getCp(tw_temp, tw_den, &tw_X[0]); }

    virtual real_t getCv(const real_t &temp, const real_t &den, const real_t *X)
    { return IdealGasMixture::getCv(tw_temp, tw_den, &tw_X[0]); }

    virtual real_t getViscosity(const real_t &temp, const real_t &den, const real_t *X)
    { return IdealGasMixture::getViscosity(tw_temp, tw_den, &tw_X[0]); }

    virtual real_t getKThermal(const real_t &temp, const real_t &den, const real_t *X)
    { return IdealGasMixture::getKThermal(tw_temp, tw_den, &tw_X[0]); }

    virtual void getDiffusionCoefficients(const real_t &temp, const real_t &den, const real_t *X, real_t *&diffusionCoefficients)
    { Mixture::getDiffusionCoefficients(tw_temp, tw_den, &tw_X[0], diffusionCoefficients); }

  private:
    real_t tw_temp, tw_den;
    std::vector<real_t> tw_X;
  };

  //
  // STAND ALONE FUNCTIONS
  //
  
  //
  // set the "mixture context" to the specified mixture instance for fortran calls
  //
  void setMixtureContext(Mixture &mixture);

  // 
  // get the mixture used by Fortran calls, this will be null if it has never been set
  //
  Mixture *getMixtureContext();

} // namespace CG

#endif
