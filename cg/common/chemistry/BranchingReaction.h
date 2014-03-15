#ifndef BRANCHING_REACTION_H
#define BRANCHING_REACTION_H
//
// Define a Branching reaction
//

#include "Reactions.h"


class BranchingReaction : public Reactions
{
 public:
  enum Species
  {
    lambda=0
  };
  
  BranchingReaction();
  ~BranchingReaction();

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
