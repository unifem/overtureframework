open a data-base
   frontValveExit.hdf
   open an old file read-only
   get all mappings from the data-base
*
open a data-base
   frontPortUpper.hdf
   open an old file read-only
   get all mappings from the data-base
*
  Box
   mappingName
    portCore
    * aspect ratios 
    specify corners
      54 18.75 127  86 60 158
    rotate
      -29.539 1
      70. 20 142.5
    lines
      25 31 25
   boundary conditions
     1 1 4 1 1 1
   share
     0 0 4 0 0 0
   exit
*
  Box
   mappingName
    exitCore
    * aspect ratios  dx=58-41=17  dz=120-150=-30 
    specify corners
      41.  32  150   55.  66. 120.   41.  32  150   51.  66. 120.
    rotate
      -29.539 1
      41.  35  150
    lines
      10 31 25
   boundary conditions
     2 1 1 1 1 1
   share
     2 0 0 0 0 0
   exit
*
  Cylinder
    orientation
      2, 0, 1
    bounds on the axial variable
     18.75 60.      18.75 62.
    bounds on the radial variable
      4.7375  7.735
    center for cylinder
      69.70 0. 142.55   69.75 0. 142.5  70. 0. 142.5
    lines
      27 21 5
    boundary conditions
      -1 -1 4 0 6 0
    share
       0  0 4 0 6 0
    mappingName
      valveStem
    exit
*
*
  view mappings


    valveStem
    valveStemExitVolume


  frontPortEndVolume
  exitCore
  y+r 110

  choose all
