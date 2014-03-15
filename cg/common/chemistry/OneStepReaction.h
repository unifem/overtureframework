#ifndef ONE_STEP_REACTION_H
#define ONE_STEP_REACTION_H
//
// Define a one step reaction
//

#include "Reactions.h"

//
// Define the reaction mechanism for the H_2 - F_2 reaction
//

class OneStepReaction : public Reactions
{
 public:
  enum Species
  {
    lambda=0
  };
  
  OneStepReaction();
  ~OneStepReaction();

  void reactionRates( const real & teS, const RealArray & kb, const RealArray & kf );
  void chemicalSource(const real & rhoS, const real & teS, const RealArray & y, const RealArray & sigma, 
                      const RealArray & sy=Overture::nullRealArray() );
  void chemicalSource(const RealArray & rhoS, const RealArray & teS, const RealArray & y, 
                      const RealArray & sigma, const RealArray & sy=Overture::nullRealArray() );

  RealArray & sigmaFromRPTY(const RealArray & rho, 
			    const RealArray & p,  
			    const RealArray & te,
			    const RealArray & y, 
			    const RealArray & sigmai );
};

#endif 
