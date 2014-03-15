open a data-base
   catPortCylinderEdge.hdf
   open an old file read-only
   get all mappings from the data-base
*
open a data-base
  catPortCylinderEdgePort.hdf
  open an old file read-only
  get all mappings from the data-base
*
open a data-base
  leftPortGrid.hdf
  open an old file read-only
  get all mappings from the data-base
*
open a data-base
  rightPortGrid.hdf
  open an old file read-only
  get all mappings from the data-base
*
  Cylinder
    mappingName
     cylinder
    orientation
      2 0 1
    bounds on the radial variable
      42.5 62.5   40 62.5   40 62    40 63  40 65   40 60
    bounds on the axial variable
      -40 0
    centre for cylinder
      78 0 105
    lines
      201 9 13  121 9 13    61 9 7
   boundary conditions
     -1 -1 1 1 0 1
   share
     0 0 4 1 0 5
    exit
*
  Box
    * aspect ratios 
    specify corners
      33 -40 60 123 0 150     35 -40 62  121 0. 148 
    lines
      61 9 61      61 13 61  41 13 41   31 21  31 
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
      23 -5 77   59 5.25  113         22 -5 77   58 5.25  113
    lines
      19 7 19 
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
      51 -5 124  88 5.25  161   52 -5 124  89 5.25  161
    lines
      19 7 19 
   mappingName
     rightPortCore
   boundary conditions
     0 0 0 3 0 0
   share
     0 0 0 3 0 0
   exit
*
  view mappings
