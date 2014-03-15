create mappings
  rectangle
    set corners
    0 5 0 1
    mapping parameters
    mappingName channel
    lines 501 101
    Share Value: bottom  1
    close mapping dialog
    exit
  annulus
    centre for annulus
    3 0
    inner and outer radii
    .3 .35
    start and end angles
    0 .5
    lines
    111 6
    boundary conditions
    1 1 1 0
    share
    1 1 0 0
    mappingName
    cylinder
    exit
  exit this menu
generate an overlapping grid
  channel
  cylinder
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
    interpolation type
      explicit for all grids
  exit
  compute overlap
*  pause
exit
*
save an overlapping grid
  cicMP2.hdf
  cicMP2
exit



create mappings
  rectangle
    set corners
    0 1 0 1
    mapping parameters
    mappingName channel
    lines 101 101
    Share Value: bottom  1
    close mapping dialog
    exit
  annulus
    centre for annulus
    0 0
    inner and outer radii
    .3 .35
    start and end angles
    0 .25
    lines
    56 6
    boundary conditions
    1 1 1 0
    share
    1 1 0 0
    mappingName
    cylinder
    exit
  exit this menu
generate an overlapping grid
  channel
  cylinder
  done
  change parameters
    interpolation type
      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
*   pause
exit
*
save an overlapping grid
  cicMP.hdf
  cicMP
exit

