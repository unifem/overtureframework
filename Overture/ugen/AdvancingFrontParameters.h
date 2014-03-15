#ifndef __ADVANCING_FRONT_PARAMETERS__
#define __ADVANCING_FRONT_PARAMETERS__

#include "Overture.h"
#include "DialogData.h"

class AdvancingFrontParameters
{
public:

  // toggleable parameters, mostly for plotting
  enum StateEnum {
    frontNodes = 0,
    frontEdges,
    frontFaces,
    meshEdges,
    meshFaces,
    lastParam
  };

  AdvancingFrontParameters( real maxang=80., real egrowth=-1.0, bool usefunc=true, int defAdvNum=-1, 
			    real qt=0.01 );
  AdvancingFrontParameters( const AdvancingFrontParameters &aparam ) : 
    maxNeighborAngle( aparam.maxNeighborAngle ), 
    edgeGrowthFactor( aparam.edgeGrowthFactor ), 
    useControlFunction( aparam.useControlFunction ),
    defaultNumberOfAdvances(aparam.defaultNumberOfAdvances),
    searchDistFactor( aparam.searchDistFactor ),
    discardDistFactor( aparam.discardDistFactor) { }
  
  ~AdvancingFrontParameters() { }

  void setMaxNeighborAngle( real ang  )    { maxNeighborAngle = ang; }
  void setEdgeGrowthFactor( real fact )    { edgeGrowthFactor = fact; }
  void toggleControlFunction()   { useControlFunction = !useControlFunction ; }
  void setNumberOfAdvances( int adv ) { defaultNumberOfAdvances = adv; }
  void setQualityTolerance( real qt ) { qualityTolerance = qt; }

  real getMaxNeighborAngle() { return maxNeighborAngle; }
  real getEdgeGrowthFactor() { return edgeGrowthFactor; }

  real getQualityTolerance() { return qualityTolerance; }

  real getAuxiliaryAngleTolerance() { return auxang; }
  void setAuxiliaryAngleTolerance(real a) { auxang = a; }

  bool usingControlFunction() { return useControlFunction; }
  int getNumberOfAdvances() { return defaultNumberOfAdvances; }

  void toggle(StateEnum t) { toggleParams[t]= !toggleParams[t]; }
  bool state(StateEnum t) const { return toggleParams[t]; }

  void highlightFace(int f) { highlightedFace = f; }
  int highlightFace() const { return highlightedFace; }

  void setSearchDistFactor( real sdf ) { searchDistFactor = sdf; }
  real getSearchDistFactor( ) const { return searchDistFactor; }

  void setDiscardDistFactor( real sdf ) { discardDistFactor = sdf; }
  real getDiscardDistFactor( ) const { return discardDistFactor; }

private: 
  real maxNeighborAngle;
  real edgeGrowthFactor;
  real auxang;
  real qualityTolerance;
  real searchDistFactor;
  real discardDistFactor;

  bool useControlFunction;
  int defaultNumberOfAdvances;
  int highlightedFace;
 
  bool toggleParams[lastParam];

};

#endif
