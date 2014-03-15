#ifndef __CG_INTERACTION_POTENTIAL_H__
#define __CG_INTERACTION_POTENTIAL_H__

namespace CG {
  class InteractionPotential;
  class LennardJonesPotential;

  typedef KK::sptr<InteractionPotential> InteractionPotentialP;
  typedef KK::sptr<LennardJonesPotential> LennardJonesPotentialP;

  // 
  // InteractionPotential encapsulates the interface to collision integrals (\Omega_{i,j})
  //
  class InteractionPotential {

  public:
    /// \Omega_{i,j} as a function of the reduced temperature T
    virtual real_t OmegaIntegral(const int &i, const int &j, real_t T) const = 0;
    /// interaction diameter in angstroms
    virtual const std::string getName() const = 0;

  private:
    
  };

  struct InteractionPotentialParameters {
    virtual bool isInitialized() const=0;
  };

  // 
  // here is a struct that materials use to maintain Lennard-Jones data
  //
  struct LennardJonesParameters {
    enum Parameters {
      WELL_DEPTH=0,
      SIGMA=1,
      NUMBER_OF_PARAMETERS
    };

    inline LennardJonesParameters() {
      data[WELL_DEPTH] = data[SIGMA] = -1;
    }

    virtual inline bool isInitialized() const { return data[WELL_DEPTH]>0 && data[SIGMA]>0; }

    inline const real_t &get( const LennardJonesParameters::Parameters &p ) const { return data[p]; }
    inline real_t &get( const LennardJonesParameters::Parameters &p ) { return data[p]; }

  private:
    real_t data[NUMBER_OF_PARAMETERS];
  };

  //
  // And here is the classic Lennard-Jones potential...
  //
  class LennardJonesPotential : public InteractionPotential {
  public:
    /// constructor, well_depth and sigma in units of Energy/temperature and length respectively
    LennardJonesPotential() {}
    virtual ~LennardJonesPotential() {}

    virtual real_t OmegaIntegral(const int &i, const int &j, real_t Ts) const;
    virtual const std::string getName() const { return name; }

  private:

    static std::string name;
  };


  //
  // Here are macros that add potential data to classes such as CG::Material.
  // For each new Potential that is added we need to add stuff to these macros
  //
#define CG_DEFINE_INTERACTION_POTENTIAL_MATERIAL_DATA \
  LennardJonesParameters LJData;

#define CG_DEFINE_INTERACTION_POTENTIAL_COPY \
  for ( int p=0; p<LennardJonesParameters::NUMBER_OF_PARAMETERS; p++ ) { LJData.get((LennardJonesParameters::Parameters) p) = mat.LJData.get((LennardJonesParameters::Parameters)p); }

#define CG_DEFINE_INTERACTION_POTENTIAL_MATERIAL_INTERFACE \
  bool hasLennardJonesData() const { return LJData.isInitialized(); }  \
  const real_t &getPotentialParameter( const LennardJonesParameters::Parameters &p ) const { return LJData.get(p);}  \
  real_t &getPotentialParameter( const LennardJonesParameters::Parameters &p ) { return LJData.get(p);}

}

#endif
