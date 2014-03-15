c #include "OvertureDefine.h"
#ifndef OV_USE_DOUBLE
      REAL FUNCTION DRMACH(I)
#else
      DOUBLE PRECISION FUNCTION DRMACH(I)
#endif
c=========================================================================
c  For Bill's version of slap - single precision version of drmach
c=========================================================================
#ifndef OV_USE_DOUBLE
      drmach=r1mach(i)
#else
      double precision d1mach
      drmach=d1mach(i)
#endif
      RETURN
      END
