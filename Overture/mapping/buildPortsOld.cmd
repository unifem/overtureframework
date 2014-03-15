  open a data-base
    catGrids.hdf
  open an old file read-only
  get from the data-base
    joinStrip
  get from the data-base
    leftPort
  get from the data-base
    rightPort
  Cylinder
    mappingName
     cylinder
    orientation
      2 0 1
    bounds on the radial variable
      50 70
    bounds on the axial variable
      -40 0
    centre for cylinder
      78 0 105
    lines
      121 9 13    61 9 7
   boundary conditions
     -1 -1 1 1 0 1
   share
     0 0 4 1 0 0
    exit
*
  Box
    * aspect ratios 106 40 106
    specify corners
      25 -40 52   131 0.  158           18 -40 45   138 0.  165
    lines
      61 13 61  41 13 41   31 21  31 
   mappingName
     cylinderCore
   boundary conditions
     0 0 4 1 0 0
   share
     0 0 4 1 0 0
   exit
*
  Box
    * aspect ratios 36 10 36
    specify corners
      22 -5 77   58 5.25  113
    lines
      17 7 17 
   mappingName
     leftPortCore
   boundary conditions
     0 0 0 2 0 0
   share
     0 0 0 2 0 0
   exit
*
  Box
    specify corners
      52 -5 124  89 5.25  161
    lines
      17 7 17 
   mappingName
     rightPortCore
   boundary conditions
     0 0 0 3 0 0
   share
     0 0 0 3 0 0
   exit
*
  view mappings
    rightPort
    rightPortCore
    leftPort
    leftPortCore
    cylinder
    cylinderCore
    joinStrip