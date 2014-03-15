*
* Build a grid for a corner with three block-structured squares and an upper non-block grid
*  
*  Notes: we use "implicit" interpolation for block grids with matching grid points.
*         Since the grid points match the interpolation will really be explicit.
*         Use explicit interpolation for non-block grids (this is optional)
*
* NOTE: the corner point is not correct on this grid : grid 2 and 3 interpolate from
*       each other (computed during boundary interpolation). We need a separate block structured
*       grid option to do this properly.
* 
$factor=1; $name="cornerThreeSquares$Factor.hdf"; 
*
$ds=.1/$factor; # grid spacing 
*
create mappings
*
* --- upper rectangle that sits on top of the 3 corner grids ---
* ---- this is not a block matching grid ----
  rectangle
    set corners
      -1. 1. .86 2. 
    lines
      41 21 
    boundary conditions
      1 2 0 4
    share
      1 2 0 0
    mappingName
      upper
    exit
* -- first block grid ---
  rectangle
    set corners
      -1. 0. 0. 1. 
    lines
      21 21 
    boundary conditions
      1 0 3 0
    share
      1 0 0 0
    mappingName
      topLeft
    exit
* 
* -- second block grid ---
  rectangle
    set corners
      0. 1. 0. 1. 
    lines
      21 21 
    boundary conditions
      0 2 0 0
    share
      0 2 0 0
    mappingName
      topRight
    exit
* 
* -- third block grid ---
  rectangle
    set corners
      0. 1. -1. 0. 
    lines
      21 21 
    boundary conditions
      1 2 3 0
    share
      0 2 0 0
    mappingName
      bottomRight
    exit
  exit this menu
  generate an overlapping grid
    upper
    topRight
    topLeft
    bottomRight
* 
    change parameters
      * turn off hole cutting between block grids 
      prevent hole cutting
       topLeft
        topRight
       topLeft
        bottomRight
* 
       topRight
        topLeft
       topRight
        bottomRight
* 
       bottomRight
        topLeft
       bottomRight
        topRight
      done
* 
    interpolation width 
      all 
      all
      5 5 5 
     discretization width
      all
      5 5 
     interpolation type
      * first set all grids explicit
      explicit for all grids
     interpolation type
      * make block structure grids "implicit interpolation"
      set implicit for some grids
       topLeft
        topRight
       topLeft
        bottomRight
* 
       topRight
        topLeft
       topRight
        bottomRight
* 
       bottomRight
        topLeft
       bottomRight
        topRight
      done
      * -- print the overlap parameters: 
      show parameter values
    exit
  compute overlap
*  pause
  exit
*
save an overlapping grid
$name
corner
exit

