C
C   Generic dimensions for Chimera Grid Tools.
C   (derived from Collar Grid Tools - needs to be updated later)
C
C   Maximum 1-, 2-, and 3-D grid sizes (used by all tools).
C
C   Note that M2D may be set smaller than M1D*M1D, specifically for
C   SURGRD, which uses a number of M2D*MRGRD arrays.
C
      PARAMETER (M1D   =     500)
      PARAMETER (M2D   =   50000)
      PARAMETER (M3D   = 1500000)
C
C   Maximum number of subsets (used by PROGRD).
C
      PARAMETER (MSUB  =      50)
C
C   Maximum number of reference grids (used by SURGRD).
C
      PARAMETER (MRGRD =      30)
C
C   Maximum number of nodes for 1D stretching function (used by SURGRD).
C
      PARAMETER (MNOD  =     200)
