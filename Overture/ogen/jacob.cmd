create mappings
  Annulus
    inner radius
    1.
    outer radius
    1.06
    lines
    200 18
    boundary conditions
    -1 -1 1 0
    mappingName
    inner annulus
    exit
  Annulus
    inner radius
    1.04
    outer radius
    1.1
    centre for annulus
    0.01 0.
    lines
    200 18
    boundary conditions
    -1 -1 0 2
    mappingName
    outer annulus
    exit
  exit this menu
  generate an overlapping grid
    outer annulus
    inner annulus
*  change parameters
*    interpolation type
*    explicit for all grids
*  exit
  * pause
  compute overlap
  exit
  save an overlapping grid
    eccentric.grid
    eccentric annuluses
  exit
