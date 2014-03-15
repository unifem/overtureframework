* =====================eccentric.cmd=====================
create mappings
  Annulus
    mappingName
      inner annulus
    inner radius
      1.
    outer radius
      1.07
    lines
      300 20
    boundary conditions
      -1 -1 1 0
    exit
  Annulus
    outer radius
      1.111111111
    inner radius
      1.03
    lines
      300 20
    boundary conditions
      -1 -1 0 2
    mappingName
      outer annulus
    exit
  exit this menu
  generate an overlapping grid
    outer annulus
    inner annulus
*     change parameters
*       ghost points
*         * outer annulus
*        all
*           1 1 1 1
*       exit
    compute overlap
    * pause
    exit
  save an overlapping grid
    eccentric.hdf
    eccentric annuluses
  exit