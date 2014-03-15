c Include single or double precision versions of the slap routines
c #include "OvertureDefine.h"
#ifndef OV_USE_DOUBLE

      include "sbcg.f"
      include "scgs.f"
      include "sgmres.f" 
      include "slapcg.f" 
      include "slaputil.f" 
      include "smset.f" 
      include "smvops.f" 

#else

      include "dbcg.f"
      include "dcgs.f"
      include "dgmres.f" 
      include "dlapcg.f" 
      include "dlaputil.f" 
      include "dmset.f" 
      include "dmvops.f" 

#endif
